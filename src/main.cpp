#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <ImGuiFileDialog.h>
#include "util.h"
#include "Action.h"
#include "gui.h"

// TODOS
// [[[0]]] make data structures and helper classes
//          - hashmap
//          - stack
//          - queue
//          - string class
//          - image class
// [[[1]]] handle actions.
//          - make a action map from int to string
//          - make a do_action general function
//          - handle both keyboard and mouse events through that

sf::RenderWindow window;
sf::Clock delta_clock;
bool running = true;
vec2 window_size(1200, 700);

int main()
{
    window.create(sf::VideoMode(window_size.x, window_size.y), "Grid", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    // window.setMouseCursorVisible(false);

    ImGui::SFML::Init(window);

    // variables that imgui uses and changes
    gui::Variables vars;

    // main loop
    while (running && window.isOpen())
    {
        // handle events
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

            // [[[1]]]

            // main keyboard events
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                // use the action map and call the do action with the appropriate action
                // ...
            }

            // main mouse events
            auto mpos = sf::Mouse::getPosition(window);
            vec2 pos(mpos.x, mpos.y);
            if (event.type == sf::Event::MouseButtonPressed)
            {
                switch (event.mouseButton.button)
                {
                // ...
                case sf::Mouse::Left:   { break; }
                case sf::Mouse::Middle: { break; }
                case sf::Mouse::Right:  { break; }
                default: { break; }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                switch (event.mouseButton.button)
                {
                // ...
                case sf::Mouse::Left:   { break; }
                case sf::Mouse::Middle: { break; }
                case sf::Mouse::Right:  { break; }
                default: { break; }
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                // ...
            }
        }

        // update the state of the program
        ImGui::SFML::Update(window, delta_clock.restart());
        ImGui::ShowDemoWindow();

        // menu bar at the top
        gui::show_menu_bar(&vars); // if alt is pressed toggle this maybe?
        if (vars.show_open_file_dialog) { gui::show_file_dialog(&vars); }

        // draw canvas and gui
        window.clear();
        // ... 
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}