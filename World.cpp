#include "World.h"

#include <GL/glew.h>
#include "math_helpers.h"
#include "Logger.h"
#include "WorldData.h"
#include "Chunk_1DArray.h"
#include <string>


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

}