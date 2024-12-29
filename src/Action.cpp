#include <map>
#include <SFML/Graphics.hpp>
#include "Variables.h"
#include "Action.h"
#include "Tools.h"
#include "Undo_Redo.h"

//..................................................................................................
Action::Action()
{
}

//..................................................................................................
Action::Action(const std::string& name, Action_type type)
    : name(name), type(type)
{
}

//..................................................................................................
Action::Action(const std::string& name, const vec2& pos, const vec2& prev_pos)
    : name(name), pos(pos), prev_pos(prev_pos)
{
}

//..................................................................................................
Action::Action(const std::string& name, Action_type type, const vec2& pos)
    : name(name), type(type), pos(pos)
{
}

//..................................................................................................
Action::Action(const std::string& name, float ticks)
    : name(name), ticks(ticks)
{
}

//..................................................................................................
std::ostream& operator<<(std::ostream& os, const Action& action)
{
    os << "(" << action.name << ", " << action.type << ", " << action.pos<< ")";
    return os;
}

// ................................................. IMPORTANT FUNC THAT USE ACTIONS .................................................
extern std::map<std::list<int>, std::string> action_map;
extern Variables vars;
extern sf::RenderWindow& window;
extern Undo_Redo undo_redo;

//..................................................................................................
// puts an action in the action map
void register_action(const std::list<int>& keys, const std::string& action_name)
{
    action_map[keys] = action_name;
}

//..................................................................................................
// does actions based on the data passed
void do_action(const Action& action)
{
    auto get_size_increment = [](i32 current_size) -> i32
    {
        if (current_size >= 1 && current_size < 10) return 1;
        if (current_size >= 10 && current_size < 50) return 5;
        if (current_size >= 50 && current_size < 100) return 10;
        if (current_size >= 100 && current_size < 500) return 50;
        return 100;
    };

    auto get_size_decrement = [](i32 current_size) -> i32
    {
        if (current_size > 1 && current_size <= 10) return 1;
        if (current_size > 10 && current_size <= 50) return 5;
        if (current_size > 50 && current_size <= 100) return 10;
        if (current_size > 100 && current_size <= 500) return 50;
        return 100;
    };

    if (action.name == "mouse_move")
    {
        if (vars.mouse_r_held && vars.canvas_focused)
        {
            const float speed_mult = 1.0f;
            vars.pan_delta = (action.pos - action.prev_pos) * (-speed_mult * vars.canvas_zoom_factor);
            vars.navigate_canvas_right_now = true;
        }
    }

    if (action.name == "mouse_scroll")
    {
        if (vars.canvas_focused)
        {
            const float percent_change = 0.05;
            float change_factor = (action.ticks > 0 ? 1 - percent_change : 1 + percent_change);
            vars.canvas_zoom_factor *= change_factor;
            vars.navigate_canvas_right_now = true;
        }
    }

    if (action.type == Action::START)
    {
        if (action.name == "left_click")
        {
            vars.mouse_l_held = true;
        }
        else if (action.name == "right_click")
        {
            vars.mouse_r_held = true;
        }
        else if (action.name == "toggle_menubar")
        {
            vars.show_menu_bar = !vars.show_menu_bar;
        }
        else if (action.name == "new")
        {
            vars.show_new_img_dialog = true;   
            vars.show_open_img_dialog = false;
            vars.show_saveas_img_dialog = false;
        }
        else if (action.name == "open")
        {
            vars.show_new_img_dialog = false;   
            vars.show_open_img_dialog = true;
            vars.show_saveas_img_dialog = false;
        }
        else if (action.name == "saveas")
        {
            vars.show_new_img_dialog = false;
            vars.show_open_img_dialog = false;
            vars.show_saveas_img_dialog = true;
        }
        else if (action.name == "undo")
        {
            undo_redo.undo();
        }
        else if (action.name == "redo")
        {
            undo_redo.redo();
        }
        else if (action.name == "reset_canvas_navigation")
        {
            vars.navigate_canvas_reset = true;
        }
        else if (action.name == "no_tool")
        {
            action.tools->current_tool = Tools::NO;
        }
        else if (action.name == "move_tool")
        {
            action.tools->current_tool = Tools::MOVE;
        }
        else if (action.name == "brush_tool")
        {
            action.tools->current_tool = Tools::BRUSH;
        }
        else if (action.name == "eraser_tool")
        {
            action.tools->current_tool = Tools::ERASER;
        }
        else if (action.name == "fill_tool")
        {
            action.tools->current_tool = Tools::FILL;
        }
        else if (action.name == "tool_size_down")
        {
            if (action.tools->current_tool == Tools::BRUSH)
            {
                int step = get_size_decrement(action.tools->brush_size);
                action.tools->brush_size = std::max(1, action.tools->brush_size - step);
            }
            else if (action.tools->current_tool == Tools::ERASER)
            {
                int step = get_size_decrement(action.tools->eraser_size);
                action.tools->eraser_size = std::max(1, action.tools->eraser_size - step);
            }
        }
        else if (action.name == "tool_size_up")
        {
            if (action.tools->current_tool == Tools::BRUSH)
            {
                int step = get_size_increment(action.tools->brush_size);
                action.tools->brush_size = std::min(2000, action.tools->brush_size + step);
            }
            else if (action.tools->current_tool == Tools::ERASER)
            {
                int step = get_size_increment(action.tools->eraser_size);
                action.tools->eraser_size = std::min(2000, action.tools->eraser_size + step);
            }
        }
    }
    else if (action.type == Action::END)
    {
        if (action.name == "left_click")
        {
            vars.mouse_l_held = false;
        }
        else if (action.name == "right_click")
        {
            vars.mouse_r_held = false;
        }
    }
}