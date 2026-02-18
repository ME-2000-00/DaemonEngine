#pragma once
#include "WorldData.h"

#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <unordered_map>


#include <unordered_map>

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




class World {
public:
    std::vector<Point> blocks;

	void render();

	int getBlock(glm::ivec3 chunkKey, glm::ivec3 blockPos, glm::ivec3 offset);
	void genStartChunks();
	std::unordered_map<std::tuple<int,int,int>, std::unique_ptr<Chunk_1DArray>, TupleHash> chunks;

};