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