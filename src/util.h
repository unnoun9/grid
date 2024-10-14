#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "vec2t.h"

const int GRID_SIZE = 32;

namespace util {
    void quit();
    void drawline(vec2 p1, vec2 p2, sf::Color color = sf::Color::White);
}