#include "World.h"

#include <GL/glew.h>
#include "math_helpers.h"
#include "Logger.h"
#include "WorldData.h"
#include "Chunk_1DArray.h"
#include <string>
#include "shader.h"
#include <gtc/type_ptr.hpp>

#include "NEnNamespace.h"
#include <chrono>

inline std::tuple<int, int, int> toChunkKey(const glm::ivec3& v) {
    return std::make_tuple(v.x, v.y, v.z);
}

inline int checkOutOfBounds(int axis) {
    if (axis > WorldData::CHUNK_WIDTH - 1) {
        return 0;
    }
    else if (axis < 0) {
        return WorldData::CHUNK_WIDTH - 1;
    }

	return axis;
}

inline std::string vec3ToString(const glm::vec3& v)
{
    return "(" +
        std::to_string(v.x) + ", " +
        std::to_string(v.y) + ", " +
        std::to_string(v.z) + ")";
}

// chunk key is current chunk that calls the function
int World::getBlock(glm::ivec3 chunkKey, glm::ivec3 blockPos, glm::ivec3 offset) {
	// get the chunk we want to check
    
    // gets the chunkkey that we want to check
	// to get this key we need to offset the chunk that we want to check by the offset of the block we want to check since thats the chunk that contains the block we want to check
	std::tuple<int, int, int> key = toChunkKey(chunkKey + offset);
	int blockType = 1; // default to out of bounds
    auto targetChunk = chunks.find(key);

	// check if the chunk exists
    if (targetChunk == chunks.end()) {
        return 1; // out of bounds
    }
    else {
		// chunk exists, get the block data and check if the block is air or not
        glm::vec3 block = glm::vec3(checkOutOfBounds(blockPos.x + offset.x), checkOutOfBounds(blockPos.y + offset.y), checkOutOfBounds(blockPos.z + offset.z));

        // check block in targetChunk
        Chunk_1DArray* TC = targetChunk->second.get();

        blockType = TC->getBlockData()[TC->index(block.x, block.y, block.z)];
    }

    return blockType;
}

glm::ivec3 World::toChunkCoords(const glm::vec3& worldPos) const {
    glm::ivec3 chunk;

    chunk.x = (int)std::floor(worldPos.x / (WorldData::CHUNK_WIDTH));
    chunk.y = (int)std::floor(worldPos.y / (WorldData::CHUNK_HEIGHT));
    chunk.z = (int)std::floor(worldPos.z / (WorldData::CHUNK_WIDTH));

    return chunk;
}

Chunk_1DArray* World::getChunk(const glm::ivec3& chunkKey) const {
    auto key = std::make_tuple(chunkKey.x, chunkKey.y, chunkKey.z);
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        return it->second.get();
    }
    return nullptr; // chunk not generated
}

void World::init() {


    std::string vsp = "test.vert";
    std::string fsp = "test.frag";

    shader = CreateShader(vsp, fsp);

    glUseProgram(shader);

    worker = std::thread(&World::genChunks, this);
}

void World::exit() {
    // cleans up
    glDeleteProgram(shader);
    // join thread?
}

void World::update() {
 //   player.world = this;
 //   auto hit = player.Raycast(glm::vec3(0,-1,0), NEngine::user_cam.pos, 1.8, 1);
	//if (hit.state != 1) {
 //       // gravity?

	//	NEngine::user_cam.pos += glm::vec3(0, -0.1f, 0);
 //   }


    // apply camera data
    int viewLoc = glGetUniformLocation(shader, "u_View");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(NEngine::user_cam.m_view));

    int projLoc = glGetUniformLocation(shader, "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(NEngine::user_cam.m_proj));

    int lightLoc = glGetUniformLocation(shader, "light");
    glUniform3fv(lightLoc, 1, glm::value_ptr(NEngine::light));

    //int tintLoc = glGetUniformLocation(shader, "tint");
    //glUniform3fv(tintLoc, 1, glm::value_ptr(NEngine::tint));
    int colorsLoc = glGetUniformLocation(shader, "colors");
    glUniform3fv(colorsLoc, 3, glm::value_ptr(NEngine::colors[0]));



    // mutex lock here
    {
        std::lock_guard<std::recursive_mutex> lock(ChunkMutex);

        for (auto& [pos, chunk] : chunks) {
            if (chunk->dirty) {
                if (chunk->vao == 0) glGenVertexArrays(1, &chunk->vao);
                if (chunk->vbo == 0) glGenBuffers(1, &chunk->vbo);

                chunk->update_buffers();
                chunk->dirty = false;
            }
        }
    }
}

void World::genChunks() {
    using namespace std;

    while (NEngine::running) {
        glm::vec3 coords = toChunkCoords(NEngine::user_cam.pos);

        for (int x = -WorldData::render_distance / 2; x < WorldData::render_distance; ++x) {
            for (int y = 0; y < WorldData::WORLD_HEIGHT; ++y) {
                for (int z = -WorldData::render_distance / 2; z < WorldData::render_distance; ++z) {
                    glm::ivec3 chunkCoords = glm::ivec3(x, y, z) + glm::ivec3(coords.x, 0, coords.z);

                    // Check if chunk exists (mutex protected read)
                    bool exists = false;
                    {
                        std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
                        exists = chunks.find(toChunkKey(chunkCoords)) != chunks.end();
                    }
                    if (exists) continue;

                    // Generate CPU mesh outside lock
                    Chunk_1DArray tempChunk(glm::vec3(chunkCoords), *this);
					if (tempChunk.ignore) continue;
                    tempChunk.build();

                    // Insert chunk safely
                    {
                        std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
                        auto key = toChunkKey(chunkCoords);
                        chunks[key] = std::make_unique<Chunk_1DArray>(std::move(tempChunk));
                        chunks[key]->dirty = true;
                    }

                    // Rebuild neighbors safely
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            for (int dz = -1; dz <= 1; ++dz) {
                                if (dx == 0 && dy == 0 && dz == 0) continue; // skip self

                                glm::ivec3 neighborCoords = chunkCoords + glm::ivec3(dx, dy, dz);
                                Chunk_1DArray* neighbor = nullptr;

                                // get pointer safely
                                {
                                    std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
                                    auto it = chunks.find(toChunkKey(neighborCoords));
                                    if (it != chunks.end()) neighbor = it->second.get();
                                }

                                // rebuild CPU mesh outside lock
                                if (neighbor) {
                                    
                                    neighbor->build();

                                    // mark dirty while locked
                                    {
                                        std::lock_guard<std::recursive_mutex> lock(ChunkMutex);
                                        neighbor->dirty = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        std::this_thread::sleep_for(5ms); // avoid CPU hog
    }
}