#pragma once
#include <gtc/noise.hpp>



namespace WorldData {
	static constexpr int CHUNK_WIDTH = 16;
	static constexpr int CHUNK_HEIGHT = CHUNK_WIDTH;
	static constexpr int CHUNK_AREA = CHUNK_WIDTH * CHUNK_HEIGHT;
	static constexpr int CHUNK_VOL = CHUNK_WIDTH * CHUNK_AREA;
	static constexpr float CHUNK_SCALE = 0.01f;

	static constexpr int WORLD_HEIGHT = 6;
	static constexpr int WORLD_SIZE = 16;

	inline float dot_size = 5.0f;


	inline float getNoise(float x, float z, float scale)
	{
		float noise = glm::perlin(glm::vec2(x, z) * scale);

		// glm::perlin returns [-1, 1]
		noise = (noise + 1.0f) * 0.5f;

		return noise; // now in [0, 1]
	}

}