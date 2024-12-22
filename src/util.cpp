#include "util.h"

//..................................................................................................
std::string util::pascal_to_title(const std::string& str)
{
    std::string title;
    for (auto c : str)
    {
        if (std::isupper(c))
        {
            if (!title.empty())
                title += ' ';
            title += std::tolower(c);
        }
        else
            title += c;
    }
    return title;
}

//..................................................................................................
std::string util::title_to_pascal(const std::string& str)
{
    std::string pascal;
    bool next_upper = true;
    for (auto c : str)
    {
        if (std::isspace(c))
            next_upper = true;
        else if (next_upper)
        {
            pascal += std::toupper(c);
            next_upper = false;
        }
        else
            pascal += c;
    }
    return pascal;
}

//..................................................................................................
ui32 util::randi(ui32 index)
{
    index = (index << 13) ^ index;
    return (index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff;
}

//..................................................................................................
float util::sqrt(float number)
{
    int i;
    float x, y;
    x = number * 0.5;
    y = number;
    i = *(int*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));

    return number * y;
}

//..................................................................................................
float util::inv_sqrt(float number)
{
    int i;
    float x, y;
    x = number * 0.5;
    y = number;
    i = *(int*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));

    return y;
}

//..................................................................................................
float util::lerp(float start, float end, float t)
{
    t = clamp(t, 0.0f, 1.0f);
    return start + (end - start) * t;
}

//..................................................................................................
sf::Color util::blend_colors(const sf::Color& source, const sf::Color& destination, float factor)
{
    sf::Color blended;
    blended.r = (ui8)(source.r * factor + destination.r * (1.0f - factor));
    blended.g = (ui8)(source.g * factor + destination.g * (1.0f - factor));
    blended.b = (ui8)(source.b * factor + destination.b * (1.0f - factor));
    blended.a = (ui8)(source.a * factor + destination.a * (1.0f - factor));
    return blended;
}

//..................................................................................................
bool util::are_colors_tolerant(const sf::Color& c1, const sf::Color& c2, ui8 tolerance)
{
    float distsq = (c1.r - c2.r) * (c1.r - c2.r) +
                   (c1.g - c2.g) * (c1.g - c2.g) +
                   (c1.b - c2.b) * (c1.b - c2.b) +
                   (c1.a - c2.a) * (c1.a - c2.a);
    return distsq <= tolerance * tolerance;
}
