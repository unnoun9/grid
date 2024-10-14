#include "Action.h"

Action::Action()
{
}

Action::Action(const std::string& name, action_type type)
    : name(name), type(type)
{
}

Action::Action(const std::string& name, const vec2& pos)
    : name(name), pos(pos)
{
}

Action::Action(const std::string& name, action_type type, const vec2& pos)
    : name(name), type(type), pos(pos)
{
}

std::ostream& operator<<(std::ostream& os, const Action& action)
{
    os << "(" << action.name << ", " << action.type << ", " << action.pos<< ")";
    return os;
}