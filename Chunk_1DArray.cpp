#include "Chunk_1DArray.h"

#include <GL/glew.h>
#include "math_helpers.h"
#include "Logger.h"


// -1 out of bounds 0 air 1 not air

int Chunk_1DArray::renderFace(glm::ivec3 block, glm::ivec3 offset) { 

	glm::vec3 posToCheck = block + offset;

	if (posToCheck.x < 0 || posToCheck.x >= WorldData::CHUNK_WIDTH  ||
		posToCheck.y < 0 || posToCheck.y >= WorldData::CHUNK_HEIGHT ||
		posToCheck.z < 0 || posToCheck.z >= WorldData::CHUNK_WIDTH   ) {
		// add neightbor chunk check here
		return world.getBlock(key, block, offset);
	}
	else {
		return block_data[index(posToCheck.x, posToCheck.y, posToCheck.z)];
	}

}



// TODO: add more than top face
void Chunk_1DArray::render() {

	glColor3f(tint.r, tint.g, tint.b); // set color to tint

	glBegin(GL_QUADS);
	for (const auto& vertex : vertices) {
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}

void Chunk_1DArray::update() {
	// update the chunk
}

void Chunk_1DArray::init() {
	bool isEmpty = true;
	
	float r_tint = MathHelpers::Random01(0.0f, 1.0f);
	float g_tint = MathHelpers::Random01(0.0f, 1.0f);
	float b_tint = MathHelpers::Random01(0.0f, 1.0f);

	tint = glm::vec3(r_tint, g_tint, b_tint);

	for (int x = 0; x < WorldData::CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < WorldData::CHUNK_WIDTH; z++)
		{
			float noise = WorldData::getNoise(x + pos.x, z + pos.z, WorldData::CHUNK_SCALE);

			float columnHeight = noise* (WorldData::CHUNK_HEIGHT * WorldData::WORLD_HEIGHT);

			for (int y = 0; y < WorldData::CHUNK_HEIGHT; y++)
			{
				if (block_data[index(x, y, z)] = (y + pos.y < columnHeight)) {
					isEmpty = false;
					block_data[index(x, y, z)] = 1;
				}
				else {
					block_data[index(x, y, z)] = 0;
					//has0 = true;
				}
			}
		}
	}

	if (isEmpty) {
		ignore = true;
		return;
	}

}

void Chunk_1DArray::build() {
	vertices.clear(); // start fresh

	for (int y = 0; y < WorldData::CHUNK_HEIGHT; ++y) {
		for (int z = 0; z < WorldData::CHUNK_WIDTH; ++z) {
			for (int x = 0; x < WorldData::CHUNK_WIDTH; ++x) {
				int data = block_data[index(x, y, z)];
				if (data == 0) continue;

				// handle faces here
				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 1, 0)) == 0) { // top face
					addVertex(x, y + 1, z);
					addVertex(x, y + 1, z + 1);
					addVertex(x + 1, y + 1, z + 1);
					addVertex(x + 1, y + 1, z);

				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, -1, 0)) == 0) { // bottom face
					addVertex(x, y, z);
					addVertex(x + 1, y, z);
					addVertex(x + 1, y, z + 1);
					addVertex(x, y, z + 1);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(-1, 0, 0)) == 0) { // left face
					addVertex(x, y, z);
					addVertex(x, y, z + 1);
					addVertex(x, y + 1, z + 1);
					addVertex(x, y + 1, z);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(1, 0, 0)) == 0) { // right face
					addVertex(x + 1, y, z);
					addVertex(x + 1, y + 1, z);
					addVertex(x + 1, y + 1, z + 1);
					addVertex(x + 1, y, z + 1);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 0, -1)) == 0) { // back face
					addVertex(x, y, z);
					addVertex(x, y + 1, z);
					addVertex(x + 1, y + 1, z);
					addVertex(x + 1, y, z);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 0, 1)) == 0) { // front face
					addVertex(x, y, z + 1);
					addVertex(x + 1, y, z + 1);
					addVertex(x + 1, y + 1, z + 1);
					addVertex(x, y + 1, z + 1);
				}
			}
		}
	}
}
