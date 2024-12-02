#pragma once
#include <SFML/Graphics.hpp>
#include "vec2t.h"

struct Raster
{
    sf::Image data;
    sf::Texture texture;
    sf::Sprite sprite;

    Raster();
    bool update_texture();
    bool loadfromfile(const std::string& path);
};