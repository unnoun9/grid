#pragma once
#include <iostream>
#include "vec2t.h"

struct Action
{
    enum action_type
    {
        NONE = -1, START, END
    };

    std::string name = "none";
    action_type type = NONE;
    vec2 pos = vec2(0, 0);

    Action();
    Action(const std::string& name, action_type type);
    Action(const std::string& name, const vec2& pos);
    Action(const std::string& name, action_type type, const vec2& pos);

    friend std::ostream& operator<<(std::ostream& os, const Action& action);
};