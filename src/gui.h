#pragma once
#include "vars.h"
#include "image.h"

namespace gui {
    void show_menu_bar(Variables&);
    void show_open_dialog(Variables&, Image&);
    void show_saveas_dialog(Variables&);
}