#pragma once
#include "Variables.h"
#include "Image.h"

// functions that call ImGui's functions to draw gui
namespace gui {
    void show_menu_bar(Variables&);
    void show_open_dialog(Variables&);
    void show_saveas_dialog(Variables&);
}