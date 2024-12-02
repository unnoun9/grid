#pragma once
#include <string>
#include "vec2t.h"

// the variables that either ImGui or events/actions change for us to use in various places
// later, all these have to be removed because global variables are a bad idea, but for now i'll stick with them
struct Variables
{
    // input-related
    vec2 mouse_pos;
    bool mouse_l_held = false;
    bool mouse_r_held = false;
    float scroll_ticks = 0;

    // un/showing gui-related
    bool show_menu_bar = true;
    bool show_new_img_dialog = false;
    bool show_open_img_dialog = false;
    bool show_saveas_img_dialog = false;

    // canvas-related
    bool navigate_canvas_right_now = false;
    bool navigate_canvas_reset = false;
    float canvas_zoom_factor = 1;
    vec2 pan_delta;
    bool canvas_focused = false;

    // image-related
    bool open_image = false;
    std::string open_path;
    bool save_image = false;
    std::string save_path;

    // shaders-related
    bool use_checker_shader = true;

    // filters-related
    bool apply_gray_scale = false;
}; 