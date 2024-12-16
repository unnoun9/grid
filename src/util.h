#pragma once
#include <cmath>
#include <string>

// contains some useful helper functions
// this will expand more
namespace util {

    // this is meant for primitive types
    template <typename T>
    T clamp(T value, T min, T max)
    {
        return std::min(std::max(value, min), max);
    }

    // converts a PascalCase string to Title Case
    std::string pascal_to_title(const std::string& str);

    // converts a Title Case string to PascalCase
    std::string title_to_pascal(const std::string& str);
}