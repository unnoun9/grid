#pragma once
#include <string>
#include "vec2t.h"

// the variables that either ImGui or events/actions change for us to use in various places
struct Variables
{
    // un/showing gui-related
    bool show_menu_bar = true;
    bool show_new_img_dialog = false;
    bool show_open_img_dialog = false;
    bool show_saveas_img_dialog = false;

    // canvas-related
    bool navigate_canvas_right_now = false;
    float scroll_ticks = 0;
    float canvas_zoom_factor = 1;
    bool pan_start = false;
    vec2 pan_delta;

    // image-related
    bool open_image = false;
    std::string open_path;
    bool save_image = false;
    std::string save_path;
    
    // filters-related
    bool apply_gray_scale = false;
}; 