#include <imgui.h>
#include <ImGuiFileDialog.h>
#include "util.h"
#include "vars.h"
#include "gui.h"

extern vec2 window_size;

void gui::show_menu_bar(Variables& vars)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                vars.show_new_img_dialog = true;
            }
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                vars.show_open_img_dialog = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As..", "Ctrl+ShiftS"))
            {
                vars.show_saveas_img_dialog = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4")) { util::quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
            {}
            if (ImGui::MenuItem("Copy", "CTRL+X"))
            {}
            if (ImGui::MenuItem("Copy", "CTRL+C"))
            {}
            if (ImGui::MenuItem("Paste", "CTRL+V"))
            {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Menu bar", "Alt", &vars.show_menu_bar));
            {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void gui::show_open_dialog(Variables& vars, Image& img)
{
    if (!vars.show_open_img_dialog) return;

    IGFD::FileDialogConfig config;
    config.path = "../";
    config.flags = ImGuiFileDialogFlags_Modal; // prevent interacting with app if dialog is open
    config.fileName = "";

    // open the dialog box
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Select an Image..", "Image Files{.png,.jpg,.jpeg,.bmp},.png,.jpg,.jpeg,.bmp", config);

    // display dialog box
    ImVec2 max_size(window_size.x, window_size.y);
    ImVec2 min_size(window_size.x * 0.5f, window_size.y * 0.5f);
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, min_size, max_size))
    {
        // if action is ok
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string open_path = ImGuiFileDialog::Instance()->GetFilePathName();
            std::cout << "Open path: " << open_path << "\n";
            // load the image
            if (!img.loadfromfile(open_path, true))
            {
                std::cerr << "Failed to open image: " << open_path << std::endl;
            }

        }
        ImGuiFileDialog::Instance()->Close();
        vars.show_open_img_dialog = false;
    }
}

// almost the same as the above open dialog function
void gui::show_saveas_dialog(Variables& vars)
{
    if (!vars.show_saveas_img_dialog) return;

    IGFD::FileDialogConfig config;
    config.path = "../";
    config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite; // prevent interacting with app if dialog is open + notify overwrite of a file
    config.fileName = "untitled.png";

    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Save Image As..", "Image Files{.png,.jpg,.jpeg,.bmp}", config);

    ImVec2 max_size(window_size.x, window_size.y);
    ImVec2 min_size(window_size.x * 0.5f, window_size.y * 0.5f);
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, min_size, max_size))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string save_path = ImGuiFileDialog::Instance()->GetFilePathName();
            std::cout << "Save path: " << save_path << "\n";
            // save the image
        }
        ImGuiFileDialog::Instance()->Close();
        vars.show_saveas_img_dialog = false;
    }
}