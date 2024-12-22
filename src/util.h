#pragma once
#include <cmath>
#include <string>
#include <SFML/Graphics/Color.hpp>

#include "int.h"

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

    // random number generator
    ui32 randi(ui32 index);

    // square root
    float sqrt(float number);
    
    // 1 / square root
    float inv_sqrt(float number);

    // linear interpolation between two values
    float lerp(float start, float end, float t);

    // blend colors
    sf::Color blend_colors(const sf::Color& source, const sf::Color& destination, float factor);

    // are colors tolerant
    bool are_colors_tolerant(const sf::Color& c1, const sf::Color& c2, ui8 tolerance);
}