#pragma once
#include <SFML/Graphics.hpp>
#include "vec2t.h"

struct Canvas;

struct Raster
{
    sf::Texture texture;
    sf::Sprite sprite;

    Raster();
    bool loadfromfile(const std::string& path, Canvas* canv=nullptr);
    void create_blank(vec2 size, const sf::Color& color);
};