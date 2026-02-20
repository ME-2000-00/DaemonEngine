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




void World::genStartChunks() {
	// generate the initial chunks around the player spawn point
    for (int i = 0; i < WorldData::WORLD_SIZE; i++) {
        for (int j = 0; j < WorldData::WORLD_HEIGHT; j++) {
            for (int k = 0; k < WorldData::WORLD_SIZE; k++) {
                chunks[{i, j, k}] = std::make_unique<Chunk_1DArray>(glm::vec3(i, j, k), *this);
            }
        }
    }

    for (int i = 0; i < WorldData::WORLD_SIZE; i++) {
        for (int j = 0; j < WorldData::WORLD_HEIGHT; j++) {
            for (int k = 0; k < WorldData::WORLD_SIZE; k++) {
				chunks[{i, j, k}]->build();
            }
        }
    }

}

void World::render() {

    player.world = this;

    if (NEngine::destroy) {
        auto hit = player.Raycast(NEngine::user_cam.forward, NEngine::user_cam.pos, 16, 1);

        if (hit.state != -1) {
            Chunk_1DArray* chunk = getChunk(hit.chunk_key);
            chunk->SetBlockData(hit.index, 0);

			for (int x = -1; x <= 1; x++) {
                for(int y = -1; y <= 1; y++) {
                    for(int z = -1; z <= 1; z++) {
                        Chunk_1DArray* chunkB = getChunk(hit.chunk_key + glm::ivec3(x,y,z));
                        if (chunkB != nullptr) {
                            chunkB->build();

                        }
                    }
                }
            }


            NEngine::destroy = false;
        }
    }

    //if (NEngine::build) {
    //    auto hit = player.Raycast(NEngine::user_cam.forward, NEngine::user_cam.pos, 16, 0);

    //    if (hit.state != -1) {
    //        Chunk_1DArray* chunk = getChunk(hit.chunk_key);
    //        chunk->SetBlockData(hit.index, 1);
    //        chunk->build();

    //        NEngine::build = false;
    //    }
    //}

        //if (NEngine::build) {
        //    Chunk_1DArray* chunk = getChunk(hit.chunk_key);
        //    chunk->SetBlockData(hit.index, 1);
        //    chunk->build();

        //    NEngine::build = false;
        //}
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

    int tintLoc = glGetUniformLocation(shader, "tint");
    glUniform3fv(tintLoc, 1, glm::value_ptr(NEngine::tint));
}

void World::init() {


    std::string vsp = "test.vert";
    std::string fsp = "test.frag";

    shader = CreateShader(vsp, fsp);

    glUseProgram(shader);

}