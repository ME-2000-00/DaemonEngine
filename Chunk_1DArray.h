#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <array>

#include "WorldData.h"
#include "math_helpers.h"
#include "World.h"

class Chunk_1DArray {
public:
    // Constructor: pos is chunk coordinates, World& ref stored for cross-chunk checks
    Chunk_1DArray(glm::vec3 pos, World& world)
        : pos(pos.x* WorldData::CHUNK_WIDTH, pos.y* WorldData::CHUNK_HEIGHT, pos.z* WorldData::CHUNK_WIDTH),
        world(world), key(pos)
    {
        init();
    }

    void init();
    void render();
    void update();
    void build();

    bool ignore = false;

    // Getter returns reference to internal block data (no copy)
    const std::array<int, WorldData::CHUNK_VOL>& getBlockData() const {
        return block_data;
    }

    void SetBlockData(int index, int value) {
        block_data[index] = value;
    }

    // Returns 1D index for x,y,z
    static int index(int x, int y, int z) {
        return x + WorldData::CHUNK_WIDTH * z + WorldData::CHUNK_AREA * y;
    }
    glm::vec3 pos;

private:
    glm::vec3 key;
    World& world;

    std::array<int, WorldData::CHUNK_VOL> block_data{};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    glm::vec3 tint;

    void addVertex(int x, int y, int z, glm::vec3 normal = glm::vec3(0,0,0)) {
        vertices.push_back(glm::vec3(x, y, z) + pos);
    }

    // 0 = air, 1 = block, -1 = out of bounds
    int renderFace(glm::ivec3 block, glm::ivec3 offset);


};
