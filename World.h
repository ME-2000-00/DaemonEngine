#pragma once
#include "WorldData.h"

#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <unordered_map>
#include "player.h"

#include <unordered_map>
#include <thread>

#include <queue>
#include <mutex>

class Chunk_1DArray;


struct Point {
	glm::vec3 position;
    int state;
};



struct TupleHash {
    size_t operator()(const std::tuple<int, int, int>& t) const {
        auto [x, y, z] = t;
        size_t h = 2166136261u; // FNV offset basis
        h = (h ^ std::hash<int>{}(x)) * 16777619;
        h = (h ^ std::hash<int>{}(y)) * 16777619;
        h = (h ^ std::hash<int>{}(z)) * 16777619;
        return h;
    }
};


struct ChunkGenTask {
    glm::ivec3 chunkKey;
};  





class World {
public:

    World() {
        init();
    }

    ~World() {
        if (worker.joinable()) {
            worker.join();
        }
    }

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) = delete;
    World& operator=(World&&) = delete;



    std::vector<Point> blocks;
    glm::vec3 pos;
    glm::vec3 dir;

    // function called by thread
    void genChunks();

    // thread
    std::thread worker;

	//std::queue<ChunkGenTask> chunkGenQueue;
    std::recursive_mutex ChunkMutex;

    Player player;

    void update();

    void exit();

    void init();

	int getBlock(glm::ivec3 chunkKey, glm::ivec3 blockPos, glm::ivec3 offset);
	std::unordered_map<std::tuple<int,int,int>, std::unique_ptr<Chunk_1DArray>, TupleHash> chunks;

    glm::ivec3 toChunkCoords(const glm::vec3& worldPos) const;
    Chunk_1DArray* getChunk(const glm::ivec3& chunkKey) const;

    unsigned int shader;
};