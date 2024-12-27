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
    inline T clamp(T value, T min, T max)
    {
        return std::min(std::max(value, min), max);
    }

    // converts a PascalCase string to Title Case
    std::string pascal_to_title(const std::string& str);

    // converts a Title Case string to PascalCase
    std::string title_to_pascal(const std::string& str);

    // random number generator
    inline ui32 randi(ui32 index)
    {
        index = (index << 13) ^ index;
        return (index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff;
    }

    // square root
    float sqrt(float number);
    
    // 1 / square root
    float inv_sqrt(float number);

    // linear interpolation between two values
    inline float lerp(float start, float end, float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return start + (end - start) * t;
    }

    // blend colors
    inline sf::Color blend_colors(const sf::Color& source, const sf::Color& destination, float factor)
    {
        sf::Color blended;
        float factorinv = 1.0f - factor;
        blended.r = (ui8)(source.r * factor + destination.r * factorinv);
        blended.g = (ui8)(source.g * factor + destination.g * factorinv);
        blended.b = (ui8)(source.b * factor + destination.b * factorinv);
        blended.a = (ui8)(source.a * factor + destination.a * factorinv);
        return blended;
    }

    // computes a normalized score for how similar two colors are (0 = identical, 1 = completely different)
    inline float color_tolerancesq(const sf::Color& color1, const sf::Color& color2)
    {
        int dr = color1.r - color2.r;
        int dg = color1.g - color2.g;
        int db = color1.b - color2.b;
        return dr * dr + dg * dg + db * db;
    }

    // are colors tolerant
    inline bool are_colors_tolerant(const sf::Color& c1, const sf::Color& c2, float tolerance)
    {
        if (tolerance == 0.0f)
        {
            return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
        }

        return color_tolerancesq(c1, c2) <= tolerance * tolerance;
    }
}