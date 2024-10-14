#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>
#include "vec2t.h"

// ############################### INCOMPLETE ###############################

// represents a single r, g, b, a color pixel
struct Pixel
{
    uint8_t r = 0, g = 0, b = 0, a = 0; // RGBA color values (32-bit)

    Pixel();
    Pixel(uint8_t r, uint8_t g, uint8_t b);
    Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Pixel(const Pixel& other);
};

// represents a raster image
struct Image
{
    // contains a dynamic array of pixels
    // + more?

    // for now...
    std::vector<uint8_t> pixels; // actual 32-bit pixels stored byte-by-byte (RGBA)
    vec2ui size;                 // (width, height) of the image
    sf::Texture texture;         // used by sprite to draw the image
    sf::Sprite sprite;           // used to draw texture to the window

    bool loadfromfile(const std::string& path, bool upload_to_texture=false);
    bool savetofile(const std::string& path);
    void upload_to_texture(sf::Texture& texture);

    uint8_t operator[] (size_t index) const;                      // usage: value = img[i]
    uint8_t& operator[] (size_t index);                           // usage: img[i] = value
    Pixel operator() (uint32_t x, uint32_t y) const;              // usage: pixel = img(x, y)
    void operator() (uint32_t x, uint32_t y, const Pixel& pixel); // usage: img(x, y) = pixel or img(x, y, pixel)
};

// a layer that contains various information
// template argument should be an image (pixel array) or some kind of data like (brush stroke)
template <typename T>
struct Layer
{
    std::string name;       // layer's name; behaves as some ID of if
    vec2 pos;               // layer's position in the canvas
    vec2 size;              // layer's size
    float opacity = 100.f;  // layer's opacity or state of transparency
    std::string blend_mode; // maybe use an enum?
    short index = 0;        // layer's z-index; specifies where in the "z-axis" the layer is in the canvas
    T element;              // some element the layer contains???; could be a raster image, a text, a selection path, etc...
    // + contains an image?

    Layer(const std::string& name, const vec2& pos = {0, 0}, const vec2& size = {0, 0}, float opacity = 100.f, short index = 0);

};

struct Image_Canvas
{
    
};