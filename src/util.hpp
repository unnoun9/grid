#pragma once
#include <cmath>

// contains some useful helper functions
// this will expand more
namespace util {

    template <typename T>
    void clamp(T& value, T min, T max)
    {
        value = std::min(std::max(value, min), max);
    }

}