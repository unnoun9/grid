#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <ImGuiFileDialog.h>
#include "image.h"
#include "util.h"
#include "Action.h"
#include "gui.h"
#include "vars.h"

// ############################### TODOS ###############################
// [[[0]]] make data structures and helper classes
//          - hashmap
//          - stack
//          - queue
//          - string class
//          - image class
// [[[2]]] image canvas, storing the image in a suitable data structure (probably a dynamic array), image layers

// ############################### MACROS ###############################
// macro to enable or disable custom debugging
#define DEBUG 1

// ############################### GLOBALS VARIABLES ###############################
// main window related stuff
sf::RenderWindow window;
sf::Clock delta_clock;
bool running = true;
vec2 window_size(1280, 720);

// actions related stuff
std::map<std::list<int>, std::string> action_map;   // maps keys or shortcuts to action names
std::list<int> currently_pressed_keys;  // to track the current shortcut / pressed keys' sequence

Variables vars; // variables that imgui and actions use and change

// ############################### FUNC DECLARATIONS ###############################
// does actions based on the data passed
void do_action(const Action& action);
// puts an action in the action map
void register_action(const std::list<int>& keys, const std::string& action_name);

int main()
{
// ############################### INITIALIZATION ###############################
    window.create(sf::VideoMode(window_size.x, window_size.y), "Grid", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    // window.setMouseCursorVisible(false);

    ImGui::SFML::Init(window);


    // testing image struct
    Image img;

    // register keybinds or shortcuts
    register_action({sf::Keyboard::LAlt}, "toggle_menubar");
    register_action({sf::Keyboard::LControl, sf::Keyboard::N}, "new");
    register_action({sf::Keyboard::LControl, sf::Keyboard::O}, "open");
    register_action({sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::S}, "save");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Z}, "undo");
    register_action({sf::Keyboard::LControl, sf::Keyboard::Y}, "redo");

// ############################### MAIN LOOP ###############################
    while (running && window.isOpen())
    {
// ############################### EVENT HANDLING ###############################
        sf::Event event;
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                util::quit();

            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    util::quit();
                
            if (event.type == sf::Event::LostFocus)
                currently_pressed_keys.clear();

            // main keyboard events
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

            // main mouse events
            auto mpos = sf::Mouse::getPosition(window);
            vec2 pos(mpos.x, mpos.y);
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

            if (event.type == sf::Event::MouseMoved)
            {
                do_action(Action("mouse_move", vec2(event.mouseMove.x, event.mouseMove.y)));
            }
        }

// ############################### UPDATE THE STATE OF THE PROGRAM ###############################
        ImGui::SFML::Update(window, delta_clock.restart());
        // ImGui::ShowDemoWindow();

        // menu bar at the top
        if (vars.show_menu_bar) gui::show_menu_bar(vars); // if alt is pressed toggle this maybe?
        if (vars.show_open_img_dialog) { gui::show_open_dialog(vars, img); }
        if (vars.show_saveas_img_dialog) { gui::show_saveas_dialog(vars); }

        // this is for debugging only
        // display the current pressed keys as they are pressed or released
#if DEBUG == 1
        ImGui::Text("Keys pressed: ");
        ImGui::SameLine();
        for (auto key : currently_pressed_keys)
        {
            ImGui::Text("%d", key);
            ImGui::SameLine();
        }
#endif

// ############################### DRAW / RENDER GUI, CANVAS, AND EVERYTHING ELSE ###############################
        window.clear();
        // ...
        
        window.draw(img.sprite);
        ImGui::SFML::Render(window);
        window.display();
    }

// ############################### DE-INITIALIZATION ###############################    
    ImGui::SFML::Shutdown();
    return 0;
}

// ############################### FUNC DEFINITIONS ###############################
void do_action(const Action& action)
{
    if (action.name == "mouse_move")
    {
    }
    // else std::cout << action.name << std::endl;

    if (action.type == Action::START)
    {
        if (action.name == "left_click")
        {
        }
        else if (action.name == "right_click")
        {
        }
        else if (action.name == "toggle_menubar")
        {
            vars.show_menu_bar = !vars.show_menu_bar;       
        }
        else if (action.name == "new")
        {
            vars.show_new_img_dialog = true;   
        }
        else if (action.name == "open")
        {
            std::cout << "open\n";
            vars.show_open_img_dialog = true;
        }
        else if (action.name == "save")
        {
            vars.show_saveas_img_dialog = true;
        }
        else if (action.name == "undo")
        {
        }
        else if (action.name == "redo")
        {
        }
    }
    else if (action.type == Action::END)
    {
        if (action.name == "left_click")
        {
        }
        else if (action.name == "right_click")
        {
        }
    }
}

void register_action(const std::list<int>& keys, const std::string& action_name)
{
    action_map[keys] = action_name;
}