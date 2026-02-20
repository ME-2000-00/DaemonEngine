#pragma once

#include "glm.hpp"
#include "HitInfo.h"
#include <vector>

class World;

class Player {
public: 

	Player(World* world) : world(world) {} 
	Player() {
		world = nullptr;
	}

	// raycasts through world meshes and then through blocks of mesh

	// first find out wich chunk it collides with 
	// second find out wich block it hits

	// dda through chunks
	// get chunk key
	
	// dda through blocks in hit chunk
	// get hit data from hit block
	std::vector<glm::vec3> points;

	HitInfo Raycast(glm::vec3 dir, glm::vec3 pos, float steps, int state);
	World* world;
};