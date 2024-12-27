#include <imgui.h>
#include <ImGuiFileDialog.h>
#include "gui.h"

void show_filter_popup(const char* popup_name, const char* slider_label, int& filter_strength, int min_value, int max_value, Filters& filters, const std::string& filter_name, bool& show_popup)
{
    if (show_popup)
    {
        ImGui::OpenPopup(popup_name);
    }
    if (ImGui::BeginPopupModal(popup_name, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SliderInt(slider_label, &filter_strength, min_value, max_value);
        if (ImGui::Button("Apply"))
        {
            filters.apply_filter(filter_name);
            ImGui::CloseCurrentPopup();
            show_popup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            show_popup = false;
        }
        ImGui::EndPopup();
    }
}

//..................................................................................................
void gui::menu_bar(Variables& vars, Filters& filters)
{
    static bool show_brightness_popup = false;
    static bool show_contrast_popup = false;
    static bool show_box_blur_popup = false;
    static bool show_gaussian_blur_popup = false;
    static bool show_pixelate_popup = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                vars.show_new_img_dialog = true;
                vars.show_open_img_dialog = false;
                vars.show_saveas_img_dialog = false;
            }
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                vars.show_new_img_dialog = false;
                vars.show_open_img_dialog = true;
                vars.show_saveas_img_dialog = false;
            }
            // if (ImGui::MenuItem("Save", "Ctrl+S")); // add this when a custom save file format (e.g .ps for Photoshop) has been implemented
            if (ImGui::MenuItem("Export..", "Ctrl+Shift+S"))
            {
                vars.show_new_img_dialog = false;
                vars.show_open_img_dialog = false;
                vars.show_saveas_img_dialog = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"));
            if (ImGui::MenuItem("Redo", "CTRL+Y"));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Menu bar", "Alt", &vars.show_menu_bar));
            ImGui::Separator();
            if (ImGui::MenuItem("Use Transparent Checker Shader", NULL, &vars.use_checker_shader));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Filters"))
        {
            if (ImGui::MenuItem("Brightness", NULL))
            {
                show_brightness_popup = true;
            }
            if (ImGui::MenuItem("Contrast", NULL))
            {
                show_contrast_popup = true;
            }
            if (ImGui::MenuItem("Gray Scale", NULL))
            {
                filters.apply_filter("GrayScale");
            }
            if (ImGui::MenuItem("Invert", NULL))
            {
                filters.apply_filter("Invert");
            }
            if (ImGui::MenuItem("Sepia", NULL))
            {
                filters.apply_filter("Sepia");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Box Blur", NULL))
            {
                show_box_blur_popup = true;
            }
            if (ImGui::MenuItem("Gaussian Blur", NULL))
            {
                show_gaussian_blur_popup = true;
            }
            // if (ImGui::MenuItem("Pixelate", NULL))
            // {
            //     show_pixelate_popup = true;
            // }
            // if (ImGui::MenuItem("Edge Detection", NULL))
            // {
            //     filters.apply_filter("EdgeDetection");
            // }
            // if (ImGui::MenuItem("Sharpen", NULL))
            // {
            //     filters.apply_filter("Sharpen");
            // }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // the filters settings popups / dialogs
    show_filter_popup("Set Brightness Strength", "Brightness Strength", filters.brightness_strength, -150, 150, filters, "Brightness", show_brightness_popup);
    show_filter_popup("Set Contrast Strength", "Contrast Strength", filters.contrast_strength, -100, 100, filters, "Contrast", show_contrast_popup);
    show_filter_popup("Set Box Blur Strength", "Box Blur Strength", filters.box_blur_strength, 1, 100, filters, "BoxBlur", show_box_blur_popup);
    show_filter_popup("Set Gaussian Blur Strength", "Gaussian Blur Strength", filters.gauss_blur_strength, 1, 100, filters, "GaussianBlur", show_gaussian_blur_popup);
    // show_filter_popup("Set Pixelate Size", "Pixelate Size", filters.pixelate_size, 1, 100, filters, "Pixelate", show_pixelate_popup);
}

//..................................................................................................
void gui::open_dialog(Variables& vars)
{
    if (!vars.show_open_img_dialog) return;

    IGFD::FileDialogConfig config;
    config.path = "./test_imgs/"; // don't forget to change this back to something like "."
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
void gui::saveas_dialog(Variables& vars)
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