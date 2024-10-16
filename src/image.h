#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "vec2t.h"


// [[[1]]], [[[2]]]


// represents a single r, g, b, a color pixel
struct Pixel
{
    ui8 r = 0, g = 0, b = 0, a = 0; // RGBA color values (32-bit)

    Pixel();
    Pixel(ui8 r, ui8 g, ui8 b);
    Pixel(ui8 r, ui8 g, ui8 b, ui8 a);
    Pixel(const Pixel& other);
};


// represents a raster image
struct Image
{
    std::vector<ui8> m_pixels;  // actual 32-bit pixels stored byte-by-byte (RGBA)
    vec2 m_size;                // (width, height) of the image
    sf::Texture m_texture;      // used by sprite to draw the image
    sf::Sprite m_sprite;        // used to draw texture to the window

    // loads the image using sf::Image into our data structure then throws sf::Image away
    bool loadfromfile(const std::string& path, bool upload_to_texture=false);

    // using sf::Image, save the pixel data to an image file
    bool savetofile(const std::string& path);

    // put the pixel data in the texture to render
    void upload_to_texture(sf::Texture* m_texture=nullptr);

    // finds the 32-bit color values and returns it in a friendly pixel structure
    Pixel get_pixel(int x, int y);

    // uses the color values in the pixel structure and sets the corresponding (x, y) pixels to that value
    void set_pixel(int x, int y, const Pixel& pix);
};