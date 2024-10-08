#pragma once

namespace gui {

    struct Variables
    {
        bool show_open_file_dialog = false;
        bool show_save_file_dialog = false;
    };

    void show_menu_bar(Variables*);
    void show_file_menu(Variables*);
    void show_file_dialog(Variables*);
}