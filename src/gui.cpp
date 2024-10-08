#include <imgui.h>
#include <ImGuiFileDialog.h>
#include "util.h"
#include "gui.h"

void gui::show_menu_bar(Variables* vars)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            show_file_menu(vars);
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
        ImGui::EndMainMenuBar();
    }
}

void gui::show_file_menu(Variables* vars)
{
    if (ImGui::MenuItem("New"))
    {
        vars->show_open_file_dialog = true;
    }
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("foo.png");
        ImGui::MenuItem("bar.jpg");
        ImGui::MenuItem("baz.bmp");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                show_file_menu(vars);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..", "Ctrl+S")) {}

    ImGui::Separator();
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    ImGui::Separator();
    if (ImGui::MenuItem("Quit", "Alt+F4")) { util::quit(); }
    ImGui::EndMenu();
}

void gui::show_file_dialog(Variables* vars)
{
    // open the dialog box
    IGFD::FileDialogConfig config;
    config.path = "../";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Select an Image", "Image{.png,.jpg,.jpeg,.bmp},.png,.jpg,.jpeg,.bmp", config);
    // display dialog box
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // if action is ok
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::cout << "File path name: " << ImGuiFileDialog::Instance()->GetFilePathName() << "\n";
            std::cout << "File path: " << ImGuiFileDialog::Instance()->GetCurrentPath() << "\n";
            // load the image and store it somewhere
            // ...
        }
        ImGuiFileDialog::Instance()->Close();
        vars->show_open_file_dialog = false;
    }
}