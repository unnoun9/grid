#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <ImGuiFileDialog.h>

#include "Action.h"
#include "gui.h"
#include "Variables.h"
#include "util.hpp"
#include "Canvas.h"
#include "filters.h"
#include <stack>


// ................................................. MACROS .................................................
#define DEBUG 1 // macro to enable or disable custom debugging


// ................................................. GLOBALS VARIABLES! .................................................
sf::RenderWindow window;
sf::Clock delta_clock;
bool running = true;
vec2 window_size(1280, 720);
std::map<std::list<i32>, std::string> action_map;   // maps keys or shortcuts to action names
std::list<i32> currently_pressed_keys;              // to track the current shortcut / pressed keys' sequence
Variables vars;                                     // variables that imgui and actions use and change


// ................................................. SOME USEFUL FUNCS .................................................
// draws a line from point p1 to point p2 of the specified color in the window
void drawline(vec2 p1, vec2 p2, sf::Color color)
{
    sf::Vertex line[] = {
        sf::Vertex(vec2(p1.x, p1.y), color),
        sf::Vertex(vec2(p2.x, p2.y), color)
    };
    window.draw(line, 2, sf::Lines);   
}

// just stops the loop and closes the window
void quit()
{
    running = false;
    window.close();
}


// ................................................. MAIN .................................................
int main()
{
    // ................................................. INITIALIZATION .................................................
    window.create(sf::VideoMode(window_size.x, window_size.y), "Grid", sf::Style::Default);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    ImGui::SFML::Init(window);

    // the one and the only, canvas!
    Canvas canvas;

    // register keybinds or shortcuts
    register_action({sf::Keyboard::LAlt}, "toggle_menubar");
    register_action({sf::Keyboard::LControl, sf::Keyboard::N}, "new");
    register_action({sf::Keyboard::LControl, sf::Keyboard::O}, "open");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::S}, "save");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Z}, "undo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Y}, "redo");

    // ................................................. MAIN LOOP .................................................
    while (running && window.isOpen())
    {
        // ................................................. EVENT HANDLING .................................................
        sf::Event event;
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // miscellaneous yet important events below
            if (event.type == sf::Event::Closed)
                quit();

            // will probably remove this when done, this is just a convenient way to exit the application
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    quit();
                
            // when the window is not focused clear the pressed keys to avoid weird behaviors
            if (event.type == sf::Event::LostFocus)
                currently_pressed_keys.clear();

            // main keyboard events below
            if (event.type == sf::Event::KeyPressed)
            {
                // remember the pressed key
                currently_pressed_keys.emplace_back(event.key.code);

                // no need to process the key if it isn't in the action map
                if (action_map.find(currently_pressed_keys) == action_map.end())
                    continue;

                // call do_action with the correct action data
                do_action(Action(action_map.at(currently_pressed_keys), Action::START));
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                // forget the released key
                currently_pressed_keys.remove(event.key.code);
            }

            // this line ignores mouse events occur on ImGui stuff
            if (ImGui::GetIO().WantCaptureMouse) { continue; }

            // main mouse events below
            auto mpos = sf::Mouse::getPosition(window);
            vec2 pos(mpos.x, mpos.y);

            // when a mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:   { do_action(Action("left_click", Action::START, pos)); break; }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::START, pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::START, pos)); break; }
                default: { break; }
                }
            }

            // when a mouse button is released from pressage
            if (event.type == sf::Event::MouseButtonReleased)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:   { do_action(Action("left_click", Action::END, pos)); break; }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::END, pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::END, pos)); break; }
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

        // ................................................. UPDATE THE STATE OF THE PROGRAM .................................................
        ImGui::SFML::Update(window, delta_clock.restart());
        ImGui::ShowDemoWindow();
        
        // open image
        if (vars.open_image)
        {
            Image* img = new Image;
            img->loadfromfile(vars.open_path, true);
            canvas.layers.emplace_back(
                canvas.default_layer_name(), window_size / 2, img->m_size,
                canvas.layers.size(), img, Layer::RASTER, 100.f, Layer::NORMAL
            );
            vars.open_image = false;
        }

        // save image
        // ...


        // grayscale filter
        if (vars.apply_gray_scale)
        {
            std::cout << "grayscaling...\n";
            Image* img = (Image*)canvas.layers[canvas.layers.size() - 1].graphic;
            filters::gray_scale(*img);
            img->upload_to_texture(nullptr);
            vars.apply_gray_scale = false;
        }

        // update the view to "navigate the canvas"
        if (vars.navigate_canvas_right_now)
        {
            canvas.zoom_factor = vars.canvas_zoom_factor;
            canvas.view_center += vars.pan_delta;
            canvas.navigate(window);
            vars.navigate_canvas_right_now = false;
        }

        // ................................................. DRAW / RENDER GUI, CANVAS, AND EVERYTHING ELSE .................................................
        window.clear();

        // menu bar at the top
        if (vars.show_menu_bar) gui::show_menu_bar(vars);
        if (vars.show_open_img_dialog) { gui::show_open_dialog(vars); }
        if (vars.show_saveas_img_dialog) { gui::show_saveas_dialog(vars); }

#if DEBUG == 1 // this is for debugging only; change the 1 to 0 to simply ignore the code wrapped in #if and #endif
        // display the current pressed keys as they are pressed or released
        std::string keys_pressed = "Keys pressed: ";
        for (auto key : currently_pressed_keys)
        {
            keys_pressed += std::to_string(key) + std::string(" ");
        }
        ImGui::Text(keys_pressed.c_str());
        // display canvas' info
        ImGui::Text("Window size: (%.1f, %.1f)", window_size.x , window_size.y);
        ImGui::Text("Canvas zoom factor: %f", canvas.zoom_factor);
        ImGui::Text("Canvas size: (%.1f, %.1f)", canvas.size.x, canvas.size.y);
        ImGui::Text("View center: (%.1f, %.1f)", canvas.view_center.x, canvas.view_center.y);
#endif

        canvas.draw(window);        
        ImGui::SFML::Render(window);
        window.display();
    }

    // ................................................. DE-INITIALIZATION .................................................    
    ImGui::SFML::Shutdown();
    return 0;
}