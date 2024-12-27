#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <ImGuiFileDialog.h>

#include "Action.h"
#include "gui.h"
#include "Variables.h"
#include "util.h"
#include "Canvas.h"
#include "Assets.h"
#include "Tools.h"
#include "Filters.h"

//................................................. MACROS .................................................
#define DEBUG 1 // macro to enable or disable custom debugging

//................................................. GLOBALS VARIABLES! .................................................
std::map<std::list<i32>, std::string> action_map;   // maps keys or shortcuts to action names
std::list<i32> currently_pressed_keys;              // to track the current shortcut / pressed keys' sequence
Variables vars;                                     // variables that imgui and actions use and change
bool shaders_available = true;                      // tracks whether shaders are available in the machine
extern const char* layer_types_str[];               // see Layer.h

//................................................. SOME USEFUL FUNCS .................................................
// draws a line from point p1 to point p2 of the specified color in the window
void drawline(vec2 p1, vec2 p2, sf::Color color, sf::RenderWindow& window)
{
    sf::Vertex line[] = {
        { vec2(p1.x, p1.y), color },
        { vec2(p2.x, p2.y), color }
    };
    window.draw(line, 2, sf::Lines);   
}

// just stops the loop and closes the window
void quit(bool& running, sf::RenderWindow& window)
{
    running = false;
    window.close();
}

//................................................. MAIN .................................................
i32 main()
{
    //................................................. INITIALIZATION .................................................
    sf::RenderWindow window(sf::VideoMode(1800, 1000), "Grid", sf::Style::Default);
    window.setKeyRepeatEnabled(false);
    sf::Clock delta_clock;
    bool running = true;
    ui64 frames = 0;
    bool first_frame = true;

    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (!sf::Shader::isAvailable())
    {
        // handle this more gracefully
        std::cerr << "Your machine does not support shaders. Without shaders, `grid` may not work properly or efficiently.\n";
        std::cerr << "Perhaps your GPU drivers need to be updated?\n";
        shaders_available = false;
    }

    // load the assets and register keybinds or shortcuts
    Assets assets;
    assets.loadfromfile("assets.txt");
    // register_action({sf::Keyboard::LAlt}, "toggle_menubar");
    register_action({sf::Keyboard::LControl, sf::Keyboard::N}, "new");
    register_action({sf::Keyboard::LControl, sf::Keyboard::O}, "open");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::S}, "saveas");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Z}, "undo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Y}, "redo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::R}, "reset_canvas_navigation");
    register_action({sf::Keyboard::N}, "no_tool");
    register_action({sf::Keyboard::V}, "move_tool");
    register_action({sf::Keyboard::B}, "brush_tool");
    register_action({sf::Keyboard::LBracket}, "tool_size_down");
    register_action({sf::Keyboard::RBracket}, "tool_size_up");
    register_action({sf::Keyboard::B}, "brush_tool");
    register_action({sf::Keyboard::E}, "eraser_tool");
    register_action({sf::Keyboard::G}, "fill_tool");

    // the one and the only, canvas!
    Canvas canvas(vec2(window.getSize().x * 0.7, window.getSize().y * 0.85));

    // da tools
    Tools tools(&canvas);
    const char* tools_tooltips[Tools::NUM_TOOLS] = {    // used by imgui tooltips
        "No tool. Literally no tool is selected at all..",
        "Move tool. Moves layers using the mouse input.",
        "Brush tool. The typical paint brush tool that can paint over layers, given its settings.",
        "Eraser tool. Erases contents of a layer.",
        "Fill tool. Flood fills a color onto a valid region of a layer.",
        "Color selection tool. Selects a region based on the color clicked."
    };
    canvas.assets = &assets;
    canvas.tools = &tools;

    // da filters
    Filters filters(&canvas);

    //................................................. MAIN LOOP .................................................
    while (running && window.isOpen())
    {
        //................................................. EVENT HANDLING .................................................
        sf::Event event;
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // when the X button is clicked
            if (event.type == sf::Event::Closed)
                quit(running, window);

            // when the window is resized update the window's view and canvas' view-related variables to  ensure no stretching occurs
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            // when the window is not focused clear the pressed keys to avoid weird behaviors
            if (event.type == sf::Event::LostFocus)
                currently_pressed_keys.clear();

            // main keyboard events below
            if (event.type == sf::Event::KeyPressed)
            {
                // will probably remove this when done; this is just a convenient way to exit the application
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (vars.show_new_img_dialog) vars.show_new_img_dialog = false;
                    else if (vars.show_open_img_dialog) vars.show_open_img_dialog = false;
                    else if (vars.show_saveas_img_dialog) vars.show_saveas_img_dialog = false;
                    else quit(running, window);

                    if (ImGuiFileDialog::Instance()->IsOpened())
                        ImGuiFileDialog::Instance()->Close();
                }

                // remember the pressed key
                currently_pressed_keys.emplace_back(event.key.code);

                // no need to process the keys if it isn't in the action map
                if (action_map.find(currently_pressed_keys) == action_map.end())
                    continue;

                // call do_action with the correct action data
                Action a = Action(action_map.at(currently_pressed_keys), Action::START);
                a.tools = &tools;
                do_action(a);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                // forget the released key
                currently_pressed_keys.remove(event.key.code);
            }

            // main mouse events below
            auto mpos = sf::Mouse::getPosition(window);
            vars.mouse_pos = vec2(mpos.x, mpos.y);

            // when a mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:
                {
                    Layer* current_layer = canvas.current_layer();
                    if (current_layer)
                    {
                        // the following is the move tool's activation logic
                        vec2 layer_size;
                        if (current_layer->type == Layer::RASTER)
                            layer_size = ((Raster*)current_layer->graphic)->data.getSize();
                        // handle other layer types
                        else;

                        sf::FloatRect bounds(current_layer->pos, layer_size);
                        if (bounds.contains(canvas.mouse_p))
                        {
                            tools.is_dragging = true;
                            tools.layer_offset = canvas.mouse_p - current_layer->pos;
                        }
                    }

                    do_action(Action("left_click", Action::START, vars.mouse_pos));
                    break;
                }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::START, vars.mouse_pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::START, vars.mouse_pos)); break; }
                default: { break; }
                }
            }

            // when a mouse button is released from pressage
            if (event.type == sf::Event::MouseButtonReleased)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:
                {
                    tools.is_dragging = false;

                    do_action(Action("left_click", Action::END, vars.mouse_pos));
                    break;
                }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::END, vars.mouse_pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::END, vars.mouse_pos)); break; }
                default: { break; }
                }
            }

            // when mouse is just moved
            if (event.type == sf::Event::MouseMoved)
            {   
                static vec2 prev_pos;
                do_action(Action("mouse_move", vec2(event.mouseMove.x, event.mouseMove.y), prev_pos));
                prev_pos = vec2(event.mouseMove.x, event.mouseMove.y);
            }
            
            // when mouse is scrolled
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                do_action(Action("mouse_scroll", event.mouseWheelScroll.delta));
            }
        }

        //................................................. UPDATE THE STATE OF THE PROGRAM, DRAW / RENDER GUI, CANVAS, AND EVERYTHING ELSE BELOW! .................................................
        ImGui::SFML::Update(window, delta_clock.restart());
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::ShowDemoWindow();

        //................................................. CANVAS DRAWING & WINDOW .................................................
        // draw canvas' stuff to the RenderTexture
        if (window.hasFocus()) canvas.draw();

        // the canvas window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        // detect whether the canvas is resized
        ImVec2 current_canv_win_size = ImGui::GetWindowSize();
        if (current_canv_win_size.x != canvas.window_size.x || current_canv_win_size.y != canvas.window_size.y)
            canvas.window_size = vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        // draw the canvas' RenderTexture
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
        ImGui::ImageButton("Canvas", canvas.window_texture.getTexture().getNativeHandle(), ImVec2(canvas.window_texture.getSize().x, canvas.window_texture.getSize().y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::PopStyleVar();
        // compute the relative coordinates for mouse position
        ImVec2 abs_mouse_p = ImGui::GetMousePos();
        ImVec2 abs_screen_p = ImGui::GetItemRectMin();
        canvas.mouse_p = vec2(abs_mouse_p.x - abs_screen_p.x, abs_mouse_p.y - abs_screen_p.y);
        canvas.mouse_p = canvas.window_texture.mapPixelToCoords((vec2i)(canvas.mouse_p));
        vars.canvas_focused = ImGui::IsItemHovered();
        window.setMouseCursorVisible(!vars.canvas_focused);
        ImGui::End();
        ImGui::PopStyleVar();

        // for now, this is just to debug mouse position relative to the canvas
        sf::CircleShape circ(5 * canvas.zoom_factor, 128);
        circ.setFillColor(sf::Color(207, 207, 196, 75));
        circ.setOutlineColor(sf::Color(196, 196, 207, 175));
        circ.setOutlineThickness(1 * canvas.zoom_factor);
        circ.setPosition(canvas.mouse_p - vec2(circ.getRadius(), circ.getRadius()));
        canvas.window_texture.draw(circ);
        // color selection tool cursor ;-;
        if (tools.current_tool == Tools::COLOR_SELECTION)
        {
            sf::Sprite cursor_sprite(assets.get_texture("6ColorSelectionTool"));
            float scale = 20 * canvas.zoom_factor / cursor_sprite.getTexture()->getSize().x;
            cursor_sprite.setScale(scale, scale);
            cursor_sprite.setOrigin(cursor_sprite.getTexture()->getSize().x / 2.0f, cursor_sprite.getTexture()->getSize().y / 2.0f);
            cursor_sprite.setPosition(canvas.mouse_p.x + circ.getRadius() * 2 + 10 * canvas.zoom_factor, 
                                           canvas.mouse_p.y - circ.getRadius() * 4);
            canvas.window_texture.draw(cursor_sprite);
        }
        canvas.window_texture.display();

        //................................................. THE LAYERS PANEL .................................................
        ImGui::Begin("Layers");
        // gives ability to customize the currently selected layer
        Layer* current_layer = canvas.current_layer();
        if (current_layer)
        {
            ImGui::Combo("Blend Mode", (i32*)&current_layer->blend, layer_blend_str, IM_ARRAYSIZE(layer_blend_str));
            ImGui::DragFloat("Opecity", &current_layer->opacity, 0.5f, 0.f, 100.f, "%.1f");
            ImGui::InputText("Name", current_layer->name, IM_ARRAYSIZE(current_layer->name));
            ImGui::Text("Layer type: %s", current_layer->type_or_blend_to_cstr());
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        }
        
        // each layer interactable widget
        for (i32 n = canvas.layers.size() - 1; n >= 0; n--)
        {
            Layer& layer = canvas.layers[n];
            if (layer.is_deleted)
                continue;

            ImGui::PushID(n);
            
            // un/hides the layer
            ImGui::Checkbox("##", &layer.is_visible);
            ImGui::SameLine();

            // duplicates the layer and puts it on top of the original layer
            if (ImGui::Button("D"))
            {
                Layer duplicate((const Layer&)layer);
                strncpy(duplicate.name, canvas.default_layer_name(), LAYER_NAME_MAX_LENGTH - 1);
                canvas.layers.insert(canvas.layers.begin() + n + 1, duplicate);
            }
            ImGui::SameLine();

            // "deletes" the layer
            if (ImGui::Button("X"))
            {
                if (canvas.current_layer_index == n)
                    canvas.current_layer_index = -1;
                else if (canvas.current_layer_index > n)
                    canvas.current_layer_index--;
                layer.is_deleted = true; // maybe add a button that deletes all the layers that are marked as deleted, so user can free the memory
            }
            ImGui::SameLine();
            
            // shows the current layer and allows it to be selected
            if (ImGui::Selectable(layer.name, canvas.current_layer_index == n))
                canvas.current_layer_index = n;

            // drag and drop functionality
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                ImGui::SetDragDropPayload("LAYER_REORDER", &n, sizeof(i32));
                ImGui::Text("Move %s", layer.name);
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LAYER_REORDER"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(i32));
                    i32 payload_n = *(const i32*)payload->Data;
                    // inserting the dragged layer to the target location and shift others accordingly
                    if (payload_n != n)
                    {
                        Layer moved_layer = std::move(canvas.layers[payload_n]);
                        canvas.layers.erase(canvas.layers.begin() + payload_n);
                        canvas.layers.insert(canvas.layers.begin() + n, std::move(moved_layer));

                        // update current layer index after the move
                        if (canvas.current_layer_index == payload_n)
                            canvas.current_layer_index = n;
                        else if (canvas.current_layer_index > payload_n && canvas.current_layer_index <= n)
                            canvas.current_layer_index--;
                        else if (canvas.current_layer_index < payload_n && canvas.current_layer_index >= n)
                            canvas.current_layer_index++;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::SameLine();
            ImGui::Text("Pos: (%.0f, %.0f)", layer.pos.x, layer.pos.y);
            ImGui::PopID();
        }

        // creates a new empty layer
        if (canvas.initialized)
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::Button("Create new layer"))
            {
                Raster* img = new Raster();
                img->data.create(canvas.size.x, canvas.size.y, sf::Color(0, 0, 0, 0));

                if (img->update_texture())
                {
                    canvas.layers.emplace_back(
                        canvas.default_layer_name(),
                        (canvas.window_size - canvas.size) / 2,
                        img, Layer::RASTER, Layer::NORMAL
                    );
                }
                else
                {
                    delete img;
                }
            }
        }
        ImGui::End();

        //................................................. THE COLOR PANEL .................................................
        ImGui::Begin("Color");
        ImGui::Spacing();
        float* color_to_edit = (float*)&(canvas.current_color == 0 ? canvas.primary_color : canvas.secondary_color);
        ImGui::ColorPicker3("##color", color_to_edit, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha);
        ImGui::Spacing();
        ImGui::ColorEdit3("##rgb", color_to_edit, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);
        ImGui::ColorEdit3("##hsv", color_to_edit, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputRGB);
        ImGui::Spacing();
        if (ImGui::ColorButton("Click to change the Primary Color", canvas.primary_color, 0, ImVec2(50, 30)))
        {
            canvas.current_color = 0;
        }
        ImGui::SameLine();
        ImGui::Text("Primary Color%s", (canvas.current_color == 0 ? " (Selected)" : ""));
        if (ImGui::ColorButton("Click to change the Secondary Color", canvas.secondary_color, 0, ImVec2(50, 30)))
        {
            canvas.current_color = 1;
        }
        ImGui::SameLine();
        ImGui::Text("Secondary Color%s", (canvas.current_color == 1 ? " (Selected)" : ""));
        ImGui::End();

        //................................................. THE TOOLS PANEL .................................................
        ImGui::Begin("Tools");
        // show each tool and allow them to be selected
        i32 i = 0;
        for (auto& [tool_name, tool_texture] : assets.texture_map)
        {
            bool is_selected = i == tools.current_tool;
            // push highlighted style if this is the current tool
            if (is_selected)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
            }

            // clicked tool is activated, rest are deactivated
            if (ImGui::ImageButton(tool_name.c_str(), tool_texture.getNativeHandle(), ImVec2(22, 22)))
                tools.current_tool = i;

            // only pop if we pushed
            if (is_selected)
                ImGui::PopStyleColor(3);

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
                ImGui::SetTooltip(tools_tooltips[i]);
            ImGui::SameLine();
            i++;
        }

        //................................................. THE TOOLS SETTINGS AREA .................................................
        ImGui::Spacing();
        if (tools.current_tool == Tools::BRUSH)
        {
            ImGui::SeparatorText("Brush Settings");
            ImGui::DragInt("Size", &tools.brush_size, 1, 1, 2000, "%dpx");
            // ImGui::SliderFloat("Hardness", &tools.brush_hardness, 0.f, 100.f, "%.1f%%");
            ImGui::SliderFloat("Opacity", &tools.brush_opacity, 0.f, 100.f, "%.1f%%");
            ImGui::Checkbox("Anti-aliasing", &tools.brush_anti_aliasing);
        }
        else if (tools.current_tool == Tools::ERASER)
        {
            ImGui::SeparatorText("Eraser Settings");
            ImGui::DragInt("Size", &tools.eraser_size, 1, 1, 2000, "%dpx");
            // ImGui::SliderFloat("Hardness", &tools.eraser_hardness, 0.f, 100.f, "%.1f%%");
            ImGui::SliderFloat("Opacity", &tools.eraser_opacity, 0.f, 100.f, "%.1f%%");
            ImGui::Checkbox("Anti-aliasing", &tools.eraser_anti_aliasing);
        }
        else if (tools.current_tool == Tools::FILL)
        {
            ImGui::SeparatorText("Fill Settings");
            ImGui::SliderFloat("Tolerance", &tools.fill_tolerance, 0.f, 255.f, "%.0f");
            ImGui::SliderFloat("Opacity", &tools.fill_opacity, 0.f, 100.f, "%.1f%%");
            // ImGui::Checkbox("Anti-aliasing", &tools.fill_anti_aliasing);
            ImGui::Checkbox("Contiguous", &tools.fill_contiguous);
        }
        ImGui::End();

        //................................................. USING THE CURRENT TOOL .................................................
        if (canvas.initialized && vars.canvas_focused) tools.use_current_tool[tools.current_tool](tools);

        //................................................. OTHER GUI STUFF LIKE MENU BAR AND DIALOGS .................................................
        if (vars.show_menu_bar) gui::menu_bar(vars, filters);
        if (vars.show_open_img_dialog) gui::open_dialog(vars);
        if (vars.show_saveas_img_dialog) gui::saveas_dialog(vars);

        //................................................. NEW IMAGE .................................................
        if (vars.show_new_img_dialog)
        {
            ImGui::OpenPopup("New Image##modal");
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            static i32 width = 1280, height = 720;
            static const char* bg[] = {"Black", "White", "Transparent", "Custom"};
            static i32 current_item = 0;
            static float bg_color[4] = { 0, 0, 0, 1 };

            if (ImGui::BeginPopupModal("New Image##modal", &vars.show_new_img_dialog))
            {
                ImGui::InputInt("Width", &width);
                ImGui::InputInt("Height", &height);
                ImGui::Combo("Background", &current_item, bg, IM_ARRAYSIZE(bg));
                if (current_item == 0)
                {
                    bg_color[0] = bg_color[1] = bg_color[2] = 0;
                    bg_color[3] = 1;
                }
                else if (current_item == 1)
                {
                    bg_color[0] = bg_color[1] = bg_color[2] = bg_color[3] = 1;
                }
                else if (current_item == 2)
                {
                    bg_color[0] = bg_color[1] = bg_color[2] = bg_color[3] = 0;
                }
                else if (current_item == 3)
                {
                    ImGui::ColorEdit4("Custom Color", bg_color);
                }
                
                ImGui::Separator();
                if (ImGui::Button("Create"))
                {
                    canvas.size = vec2(width, height);
                    canvas.view_center = canvas.window_size / 2;
                    float image_scale = std::max(
                        canvas.size.x / canvas.window_size.x,
                        canvas.size.y / canvas.window_size.y
                    );
                    canvas.relative_zoom_factor = canvas.zoom_factor = vars.canvas_zoom_factor = 2.5 * image_scale;
                    canvas.navigate();
                    canvas.initialized = true;

                    canvas.layers.clear();
                    Raster* img = new Raster();
                    img->data.create(width, height, sf::Color(
                        (ui8)(bg_color[0] * 255),
                        (ui8)(bg_color[1] * 255),
                        (ui8)(bg_color[2] * 255),
                        (ui8)(bg_color[3] * 255)
                    ));

                    if (img->update_texture())
                    {
                        canvas.layers.emplace_back(
                            "layer 1",
                            (canvas.window_size - canvas.size) / 2,
                            img, Layer::RASTER, Layer::NORMAL
                        );
                        canvas.current_layer_index = canvas.layers.size() - 1;
                    }
                    else
                    {
                        delete img;
                    }

                    vars.show_new_img_dialog = false;
                    ImGui::CloseCurrentPopup();
                }         
                ImGui::EndPopup();
            }
        }

        //................................................. OPEN IMAGE .................................................
        // in a separate canvas window?
        // (maybe ask the user a question to see whether they want to open the image in a new canvas or in the same)
        // (if in the same canvas then the following behaviour is correct, i think)
        // (otherwise, maybe create a new canvas variable and push it to std::vector<Canvas>??)
        if (vars.open_image)
        {
            Raster* img = new Raster();
            if (img->loadfromfile(vars.open_path))
            {
                vec2 img_size = img->data.getSize();

                // the first opened image defines the canvas size and other variables
                if (canvas.initialized == false)
                {
                    canvas.size = img_size;
                    canvas.view_center = canvas.window_size / 2;
                    float image_scale = std::max(
                        canvas.size.x / canvas.window_size.x,
                        canvas.size.y / canvas.window_size.y
                    );
                    canvas.relative_zoom_factor = canvas.zoom_factor = vars.canvas_zoom_factor = 2.5 * image_scale;
                    canvas.navigate();
                    canvas.initialized = true;
                }

                vec2 img_pos = (canvas.window_size - img_size) / 2;

                // add to canvas' layers
                canvas.layers.emplace_back(
                    canvas.default_layer_name(),
                    img_pos,
                    img, Layer::RASTER, Layer::NORMAL
                );
                canvas.current_layer_index = canvas.layers.size() - 1;
            }
            else
                delete img;
            vars.open_image = false;
        }

        //................................................. SAVE IMAGE .................................................
        if (vars.save_image)
        {
            if (canvas.layers.empty())
            {
                std::cout << "there is no image to save...\n";
            }
            else
            {
                canvas.texture.getTexture().copyToImage().saveToFile(vars.save_path);            
                std::cout << "Saving image to: " << vars.save_path << std::endl;
            }
            vars.save_image = false;
        }

        //................................................. CANVAS NAVIGATION .................................................
        // update the view to "navigate the canvas"
        // but prevent navigating if mouse not over the canvas window or if a modal window is there (see Action.cpp where this prevention occurs)        
        // prevent navigation when canvas is not initialized as well
        vars.canvas_focused = vars.canvas_focused && (!vars.show_open_img_dialog && !vars.show_saveas_img_dialog);
        if (canvas.initialized || true)
        {
            if (vars.navigate_canvas_right_now)
            {
                canvas.zoom_factor = vars.canvas_zoom_factor;
                canvas.view_center += vars.pan_delta;
                vars.pan_delta = vec2(0, 0);
                canvas.navigate();
                vars.navigate_canvas_right_now = false;
            }

            // if asked to reset the canvas navigation
            if (vars.navigate_canvas_reset)
            {
                vars.canvas_zoom_factor = canvas.zoom_factor = canvas.relative_zoom_factor;
                canvas.view_center = canvas.window_size / 2;
                canvas.navigate();
                vars.navigate_canvas_reset = false;
            }
        }
        else
        {
            vars.canvas_zoom_factor = 1;
            vars.pan_delta *= canvas.initialized;
        }

        //................................................. DEBUG WINDOW .................................................
#if DEBUG == 1 // this is for debugging only
        // display the current pressed keys as they are pressed or released
        ImGui::Begin("Debug Information");
        std::string keys_pressed = "Keys pressed: ";
        for (auto key : currently_pressed_keys)
        {
            keys_pressed += std::to_string(key) + std::string(" ");
        }
        ImGui::Text(keys_pressed.c_str());
        // display window, canvas, view, etc's info
        ImGui::Text("Window size: (%i, %i)", window.getSize().x , window.getSize().y);
        ImGui::Separator();
        ImGui::Text("Canvas window size: (%.1f, %.1f)", canvas.window_size.x, canvas.window_size.y);
        ImGui::Text("Canvas texture size: (%i, %i)", canvas.window_texture.getSize().x , canvas.window_texture.getSize().y);
        ImGui::Text("Canvas size: (%.1f, %.1f)", canvas.size.x, canvas.size.y);
        ImGui::Text("Canvas start position: (%.1f, %.1f)", canvas.start_pos.x, canvas.start_pos.y);
        ImGui::Text("Canvas zoom factor: %f", canvas.zoom_factor);
        ImGui::Text("Canvas relative zoom factor: %f", canvas.relative_zoom_factor);
        ImGui::Text("Canvas view center: (%.1f, %.1f)", canvas.view_center.x, canvas.view_center.y);
        ImGui::Text("Pan delta: (%.1f, %.1f)", vars.pan_delta.x, vars.pan_delta.y);
        ImGui::Text("Canvas focused: %i", vars.canvas_focused);
        ImGui::Separator();
        ImGui::Text("Mouse position: (%.1f, %.1f)", vars.mouse_pos.x, vars.mouse_pos.y);
        ImGui::Text("Mouse canvas world position: (%.1f, %.1f)", canvas.mouse_p.x, canvas.mouse_p.y);
        ImGui::Text("Mouse left held: %i", vars.mouse_l_held);
        ImGui::Text("Mouse right held: %i", vars.mouse_r_held);
        ImGui::Separator();
        ImGui::Text("Current layer index: %i", canvas.current_layer_index);
        Layer* l = canvas.current_layer();
        ImGui::Text("Current layer name: %s", l ? l->name : "*NULL*");
        ImGui::End();
#endif

        //................................................. END OF FRAME .................................................
        ImGui::SFML::Render(window);
        window.display();
        frames++;
    }

    //................................................. DE-INITIALIZATION .................................................    
    ImGui::SFML::Shutdown();
    return 0;
}