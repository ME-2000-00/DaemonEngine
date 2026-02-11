#pragma once
#include <random>



inline float Random01(float min = 0.0, float max = 1.0) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0, 1.0);

    return dist(gen) * max;
}