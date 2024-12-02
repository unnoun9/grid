#pragma once
#include <cmath>

// contains some useful helper functions
// this will expand more
namespace util {

    // this is meant for primitive types
    template <typename T>
    T clamp(T value, T min, T max)
    {
        return std::min(std::max(value, min), max);
    }

}