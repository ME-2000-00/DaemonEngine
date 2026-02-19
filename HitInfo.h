#pragma once
#include "glm.hpp"




struct HitInfo {
	glm::ivec3 Ipos;
	glm::vec3 Fpos;

	glm::ivec3 chunk_key;

	glm::ivec3 block;
	int state;

	float dist;

	int index;
};