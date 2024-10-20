#include <imgui.h>
#include <ImGuiFileDialog.h>
#include "gui.h"

//..................................................................................................
// the main menu bar
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
            if (ImGui::MenuItem("Save", "Ctrl+S"));
            if (ImGui::MenuItem("Save As..", "Ctrl+ShiftS"))
            {
                vars.show_saveas_img_dialog = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"));
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false));
            if (ImGui::MenuItem("Copy", "CTRL+X"));
            if (ImGui::MenuItem("Copy", "CTRL+C"));
            if (ImGui::MenuItem("Paste", "CTRL+V"));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Menu bar", "Alt", &vars.show_menu_bar));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Filters"))
        {
            if (ImGui::MenuItem("Gray Scale", NULL, &vars.apply_gray_scale));
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

//..................................................................................................
// open a dialog that opens an image
void gui::show_open_dialog(Variables& vars)
{
    if (!vars.show_open_img_dialog) return;

    IGFD::FileDialogConfig config;
    config.path = ".";
    config.flags = ImGuiFileDialogFlags_Modal; // prevent interacting with app if dialog is open
    config.fileName = "";

    // open the dialog box
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Select an Image..", "Image Files{.png,.jpg,.jpeg,.bmp},.png,.jpg,.jpeg,.bmp", config);

    // display dialog box
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse))
    {
        // if action is ok
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            // load and open the image
            vars.open_path = ImGuiFileDialog::Instance()->GetFilePathName();
            vars.open_image = true;
        }
        ImGuiFileDialog::Instance()->Close();
        vars.show_open_img_dialog = false;
    }
}

//..................................................................................................
// almost the same as the above open dialog function
void gui::show_saveas_dialog(Variables& vars)
{
    if (!vars.show_saveas_img_dialog) return;

    IGFD::FileDialogConfig config;
    config.path = ".";
    config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite; // prevent interacting with app if dialog is open + notify overwrite of a file
    config.fileName = "untitled.png";

    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Save Image As..", "Image Files{.png,.jpg,.jpeg,.bmp}", config);

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            // save the image
            vars.save_path = ImGuiFileDialog::Instance()->GetFilePathName();
            vars.save_image = true;
        }
        ImGuiFileDialog::Instance()->Close();
        vars.show_saveas_img_dialog = false;
    }
}