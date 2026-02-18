#pragma once
#include <random>


namespace MathHelpers {
    inline float Random01(float min = 0.0, float max = 1.0) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0, 1.0);

        return dist(gen) * max;
    }



    inline int floorDiv(int a, int b)
    {
        int result = a / b;
        if ((a ^ b) < 0 && a % b != 0)
            result--;
        return result;
    }
}
