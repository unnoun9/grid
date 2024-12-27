#pragma once
#include "Variables.h"
#include "Filters.h"

// functions that call ImGui's functions to draw gui
namespace gui {
    void menu_bar(Variables&, Filters&);
    void open_dialog(Variables&);
    void saveas_dialog(Variables&);
}