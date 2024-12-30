#pragma once
#include "Variables.h"
#include "Filters.h"

struct Undo_redo;

// functions that call ImGui's functions to draw gui
namespace gui {
    void menu_bar(Variables&, Filters&, Undo_redo&);
    void open_dialog(Variables&);
    void saveas_dialog(Variables&);
}