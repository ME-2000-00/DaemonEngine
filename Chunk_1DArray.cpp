#include "Chunk_1DArray.h"

#include <GL/glew.h>
#include "math_helpers.h"
#include "Logger.h"

#include "NEnNamespace.h"
#include <gtc/type_ptr.hpp>

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
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Chunk_1DArray::update() {
	// update the chunk

}

void Chunk_1DArray::init() {
	bool isEmpty = true;
	bool has0 = false;
	
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
				if (y + pos.y < columnHeight || y + pos.y < (WorldData::CHUNK_HEIGHT * (WorldData::WORLD_HEIGHT * .4)) + 1) {
					if (y + pos.y > columnHeight - 1) {
						block_data[index(x, y, z)] = 1;
					}
					else {
						block_data[index(x, y, z)] = 2;
					}

					isEmpty = false;
				}
				else {
					block_data[index(x, y, z)] = 0;
					has0 = true;
				}
			}
		}
	}

	if (isEmpty && !has0) {
		ignore = true;
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
				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 1, 0)) == 0) {

					// triangle 1
					addVertex(x, y + 1, z, glm::vec3(0,1,0), data);
					addVertex(x, y + 1, z + 1, glm::vec3(0, 1, 0), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(0, 1, 0), data);

					// triangle 2
					addVertex(x, y + 1, z, glm::vec3(0, 1, 0), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(0, 1, 0), data);
					addVertex(x + 1, y + 1, z, glm::vec3(0, 1, 0), data);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, -1, 0)) == 0) {

					// triangle 1
					addVertex(x, y, z, glm::vec3(0, -1, 0), data);
					addVertex(x + 1, y, z, glm::vec3(0, -1, 0), data);
					addVertex(x + 1, y, z + 1, glm::vec3(0, -1, 0), data);

					// triangle 2
					addVertex(x, y, z, glm::vec3(0, -1, 0), data);
					addVertex(x + 1, y, z + 1, glm::vec3(0, -1, 0), data);
					addVertex(x, y, z + 1, glm::vec3(0, -1, 0), data);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(-1, 0, 0)) == 0) {

					// triangle 1
					addVertex(x, y, z, glm::vec3(-1, 0, 0), data);
					addVertex(x, y, z + 1, glm::vec3(-1, 0, 0), data);
					addVertex(x, y + 1, z + 1, glm::vec3(-1, 0, 0), data);

					// triangle 2
					addVertex(x, y, z, glm::vec3(-1, 0, 0), data);
					addVertex(x, y + 1, z + 1, glm::vec3(-1, 0, 0), data);
					addVertex(x, y + 1, z, glm::vec3(-1, 0, 0), data);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(1, 0, 0)) == 0) {

					// triangle 1
					addVertex(x + 1, y, z, glm::vec3(1, 0, 0), data);
					addVertex(x + 1, y + 1, z, glm::vec3(1, 0, 0), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(1, 0, 0), data);

					// triangle 2
					addVertex(x + 1, y, z, glm::vec3(1, 0, 0), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(1, 0, 0), data);
					addVertex(x + 1, y, z + 1, glm::vec3(1, 0, 0), data);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 0, -1)) == 0) {

					// triangle 1
					addVertex(x, y, z, glm::vec3(0, 0, -1), data);
					addVertex(x, y + 1, z, glm::vec3(0, 0, -1), data);
					addVertex(x + 1, y + 1, z, glm::vec3(0, 0, -1), data);

					// triangle 2
					addVertex(x, y, z, glm::vec3(0, 0, -1), data);
					addVertex(x + 1, y + 1, z, glm::vec3(0, 0, -1), data);
					addVertex(x + 1, y, z, glm::vec3(0, 0, -1), data);
				}

				if (renderFace(glm::ivec3(x, y, z), glm::ivec3(0, 0, 1)) == 0) {

					// triangle 1
					addVertex(x, y, z + 1, glm::vec3(0, 0, 1), data);
					addVertex(x + 1, y, z + 1, glm::vec3(0, 0, 1), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(0, 0, 1), data);

					// triangle 2
					addVertex(x, y, z + 1, glm::vec3(0, 0, 1), data);
					addVertex(x + 1, y + 1, z + 1, glm::vec3(0, 0, 1), data);
					addVertex(x, y + 1, z + 1, glm::vec3(0, 0, 1), data);
				}
			}
		}
	}
}


void Chunk_1DArray::update_buffers() {

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex),
		vertices.data(),
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, state));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Chunk_1DArray::cleanup() {

}