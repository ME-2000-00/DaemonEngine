#include "Player.h"
#include "WorldData.h"
#include "World.h"
#include "Logger.h"
#include "Chunk_1DArray.h"


// DDA?  2D though
// hyp = sqrt((x2+x1)^2 + (y2+Y1)^2)

// sin cos tan atan
//  g   a   g   a
//  h   h   a   g

// h = hyp
// g = sin * h
// a = cos * h

// sin = g/h
// cos = a/h


// dx = (x2+x1)^2
// dy = (y2+Y1)^2

// Sx = sqrt( 1uint^2 + (dy/dx)^2 )
// Sy = sqrt( 1uint^2 + (dx/dy)^2 )










int clampToChunk(int axis) {
	int times = axis / WorldData::CHUNK_WIDTH;
	return axis - (times * WorldData::CHUNK_WIDTH);
}




// simple bruteforce raycast
HitInfo Player::Raycast(glm::vec3 dir, glm::vec3 start, int steps, int state) {
	points.clear();

	HitInfo hitInfo;
	hitInfo.state = -1;
	
	for (float step = 0; step <= steps; step += 0.1f) {

		// figure out the current chunk we are in 
		//world->player.points.push_back(start + dir * (float)step);
		glm::vec3 point = start + dir * step;	
		glm::ivec3 chunkkey = world->toChunkCoords(point);
	
		Chunk_1DArray* chunk = world->getChunk(chunkkey);

		if (chunk != nullptr) {
			glm::vec3 block(
				clampToChunk(point.x),
				clampToChunk(point.y),
				clampToChunk(point.z)
			);
			if (chunk->getBlockData()[chunk->index(block.x,block.y,block.z)] == state) {

				hitInfo.Ipos = block + chunk->pos + glm::vec3(-.5, .5, .5);
				hitInfo.Fpos = point;
				hitInfo.chunk_key = chunkkey;
				hitInfo.block = block;
				hitInfo.state = chunk->getBlockData()[chunk->index(block.x, block.y, block.z)];
				hitInfo.dist = glm::distance(start, point);
				hitInfo.index = chunk->index(block.x, block.y, block.z);

				break;
			}
			// handle checking for air blocks so we can properly implement placing and breaking u know
		}
	}
	return hitInfo;
}
