#include "util.h"

// converts a PascalCase string to Title Case
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

// converts a Title Case string to PascalCase
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