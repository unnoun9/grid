#pragma once
#include <iostream>
#include <list>
#include "vec2t.h"


// holds data related to user inputs
struct Action
{
    enum Action_type
    {
        NONE = -1, START, END
    };

    std::string name = "none";  // specifies which kind of action it is; enum could be used here but whatever
    Action_type type = NONE;    // specifies the type - pressed or released (for mouse clicks and keyboard events) - of the action
    vec2 prev_pos = vec2(0, 0); // the mouse position in the previous frame (main loop iteration)
    vec2 pos = vec2(0, 0);      // the mouse position of the event was a mouse event
    float ticks = 0;            // the number of ticks scrolled by the mouse scroll wheel

    Action();
    Action(const std::string& name, Action_type type);
    Action(const std::string& name, const vec2& pos, const vec2& prev_pos);
    Action(const std::string& name, Action_type type, const vec2& pos);
    Action(const std::string& name, float ticks);

    friend std::ostream& operator<<(std::ostream& os, const Action& action);
};

// puts an action in the action map
void register_action(const std::list<int>& keys, const std::string& action_name);

// does actions based on the data passed
void do_action(const Action& action);
