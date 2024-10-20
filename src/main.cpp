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

//................................................. MACROS .................................................
#define DEBUG 1 // macro to enable or disable custom debugging

//................................................. GLOBALS VARIABLES! .................................................
std::map<std::list<i32>, std::string> action_map;   // maps keys or shortcuts to action names
std::list<i32> currently_pressed_keys;              // to track the current shortcut / pressed keys' sequence
Variables vars;                                     // variables that imgui and actions use and change
bool shaders_available = true;

//................................................. SOME USEFUL FUNCS .................................................
// draws a line from point p1 to point p2 of the specified color in the window
void drawline(vec2 p1, vec2 p2, sf::Color color, sf::RenderWindow& window)
{
    sf::Vertex line[] = {
        sf::Vertex(vec2(p1.x, p1.y), color),
        sf::Vertex(vec2(p2.x, p2.y), color)
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
int main()
{
    //................................................. INITIALIZATION .................................................
    sf::RenderWindow window(sf::VideoMode(1500, 800), "Grid", sf::Style::Default);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    sf::Clock delta_clock;
    bool running = true;
    ui64 frames = 0;
    bool first_frame = true;

    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    vec2 mouse_pos;

    if (!sf::Shader::isAvailable())
    {
        // handle this more gracefully
        std::cerr << "Your machine does not support shaders. Without shaders, `grid` may not work properly or efficiently.\n";
        std::cerr << "Perhaps your GPU drivers needs to be updated?\n";
        shaders_available = false;
    }

    // the one and the only, canvas!
    Canvas canvas(vec2(window.getSize().x * 0.75, window.getSize().y * 0.9));

    // register keybinds or shortcuts
    register_action({sf::Keyboard::LAlt}, "toggle_menubar");
    register_action({sf::Keyboard::LControl, sf::Keyboard::N}, "new");
    register_action({sf::Keyboard::LControl, sf::Keyboard::O}, "open");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::S}, "save");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Z}, "undo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Y}, "redo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::R}, "reset_canvas_navigation");

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

            // will probably remove this when done, this is just a convenient way to exit the application
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
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

            // main mouse events below
            auto mpos = sf::Mouse::getPosition(window);
            mouse_pos = vec2(mpos.x, mpos.y);

            // when a mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:   { do_action(Action("left_click", Action::START, mouse_pos)); break; }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::START, mouse_pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::START, mouse_pos)); break; }
                default: { break; }
                }
            }

            // when a mouse button is released from pressage
            if (event.type == sf::Event::MouseButtonReleased)
            {
                switch (event.mouseButton.button)
                {
                case sf::Mouse::Left:   { do_action(Action("left_click", Action::END, mouse_pos)); break; }
                case sf::Mouse::Middle: { do_action(Action("middle_click", Action::END, mouse_pos)); break; }
                case sf::Mouse::Right:  { do_action(Action("right_click", Action::END, mouse_pos)); break; }
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

        //................................................. UPDATE THE STATE OF THE PROGRAM, DRAW / RENDER GUI, CANVAS, AND EVERYTHING ELSE .................................................
        ImGui::SFML::Update(window, delta_clock.restart());
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::ShowDemoWindow();
        
        // canvas
        canvas.draw();

        // canvas window
        ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImVec2 current_canv_win_size = ImGui::GetWindowSize();
        if (current_canv_win_size.x != canvas.window_size.x || current_canv_win_size.y != canvas.window_size.y)
            canvas.window_size = vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        ImGui::Image(canvas.texture.getTexture().getNativeHandle(), ImVec2(canvas.texture.getSize().x, canvas.texture.getSize().y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        // open image (in a separate canvas window?)
        if (vars.open_image)
        {
            // load the image
            Raster* img = new Raster(canvas.window_size / 2);
            img->loadfromfile(vars.open_path, true);

            // update the canvas variables and find the correct zoom factor
            canvas.size = img->m_size;
            float image_scale = std::min(
                canvas.window_size.x / canvas.size.x, canvas.window_size.y / canvas.size.y
            );
            canvas.view_center = canvas.window_size / 2;
            canvas.relative_zoom_factor = canvas.zoom_factor = vars.canvas_zoom_factor =  1.f / image_scale;
            canvas.navigate();

            // add to canvas' layers
            canvas.layers.emplace_back(
                canvas.default_layer_name(), canvas.window_size / 2, img->m_size,
                canvas.layers.size(), img, Layer::RASTER, 100.f, Layer::NORMAL
            );

            vars.open_image = false;
        }

        // save image
        // ...
        // ...
        // ...


        // grayscale filter
        if (vars.apply_gray_scale)
        {
            if (canvas.layers.empty())
            {
                std::cout << "there is no image to grayscale...\n";
            }
            else
            {
                Raster* img = (Raster*)canvas.layers[canvas.layers.size() - 1].graphic;
                filters::gray_scale(*img);
                img->upload_to_texture(nullptr);
            }
            vars.apply_gray_scale = false;
        }

        // if asked to reset the canvas navigation
        if (vars.navigate_canvas_reset)
        {
            vars.canvas_zoom_factor = canvas.zoom_factor = canvas.relative_zoom_factor;
            canvas.view_center = canvas.window_size / 2;
            canvas.navigate();
            vars.navigate_canvas_reset = false;
        }

        // update the view to "navigate the canvas"
        if (vars.navigate_canvas_right_now)
        {
            canvas.zoom_factor = vars.canvas_zoom_factor;
            canvas.view_center += vars.pan_delta;
            canvas.navigate();
            vars.navigate_canvas_right_now = false;
        }

        // menu bar at the top
        if (vars.show_menu_bar) gui::show_menu_bar(vars);
        if (vars.show_open_img_dialog) { gui::show_open_dialog(vars); }
        if (vars.show_saveas_img_dialog) { gui::show_saveas_dialog(vars); }

#if DEBUG == 1 // this is for debugging only; change the 1 to 0 to simply ignore the code wrapped in #if and #endif
        // display the current pressed keys as they are pressed or released
        ImGui::Begin("Debug");
        std::string keys_pressed = "Keys pressed: ";
        for (auto key : currently_pressed_keys)
        {
            keys_pressed += std::to_string(key) + std::string(" ");
        }
        ImGui::Text(keys_pressed.c_str());
        // display window, canvas, view, etc's info
        ImGui::Text("Window size: (%i, %i)", window.getSize().x , window.getSize().y);
        ImGui::Text("Canvas window size: (%.1f, %.1f)", canvas.window_size.x, canvas.window_size.y);
        ImGui::Text("Canvas texture size: (%i, %i)", canvas.texture.getSize().x , canvas.texture.getSize().y);
        ImGui::Text("Canvas size: (%.1f, %.1f)", canvas.size.x, canvas.size.y);
        ImGui::Text("Canvas zoom factor: %f", canvas.zoom_factor);
        ImGui::Text("Canvas view center: (%.1f, %.1f)", canvas.view_center.x, canvas.view_center.y);
        ImGui::Text("Mouse position: (%.1f, %.1f)", mouse_pos.x, mouse_pos.y);
        ImGui::End();
#endif

        ImGui::SFML::Render(window);
        window.display();
        frames++;
    }

    //................................................. DE-INITIALIZATION .................................................    
    ImGui::SFML::Shutdown();
    return 0;
}