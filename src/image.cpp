#include "image.h"

// [[[1]]], [[[2]]]

// ############################### PIXEL ###############################
Pixel::Pixel()
{
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a)
{
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b)
   : r(r), g(g), b(b), a(255)
{
}

Pixel::Pixel(const Pixel& other)
   : r(other.r), g(other.g), b(other.b), a(other.a)
{
}

// ############################### IMAGE ###############################
// loads the image using sf::Image into our data structure then throws sf::Image away
bool Image::loadfromfile(const std::string& path, bool uploadtotexture)
{
    sf::Image sfimage;
    if (!sfimage.loadFromFile(path))
    {
        std::cerr << "Could not load image: " << path << std::endl;
        return false;
    }

    size = sfimage.getSize();
    pixels.resize(size.x * size.y * 4);
    memcpy(pixels.data(), sfimage.getPixelsPtr(), pixels.size());

    if (uploadtotexture) upload_to_texture(texture);

    return true;
}

// using sf::Image, save the pixel data to an image file
bool Image::savetofile(const std::string& path)
{
    sf::Image sfimage;
    sfimage.create(size.x, size.y, pixels.data());
    if (!sfimage.saveToFile(path))
    {
        std::cerr << "Failed to save image at: " << path << std::endl;
        return false;
    }
    return true;
}

// put the pixel data in the texture to render
void Image::upload_to_texture(sf::Texture& texture)
{
    if (!texture.create(size.x, size.y))
    {
        std::cerr << "Failed to create texture of size: " << size << std::endl;
        return;
    }
    texture.update(pixels.data());
    sprite.setTexture(texture, true);
}

// operators below
uint8_t Image::operator[](size_t index) const
{
    return pixels[index];
}

uint8_t& Image::operator[](size_t index)
{
    return pixels[index];
}

Pixel Image::operator() (uint32_t x, uint32_t y) const
{
    if (x < 0 || y < 0 || x > size.x || y > size.y)
    {
        std::cerr << "Accessing pixels of image OOB at: (" << x << ", " << y << ")\n";
        return Pixel();
    }
    size_t index = (y * size.x + x) * 4;
    return Pixel(pixels[index], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
}

void Image::operator() (uint32_t x, uint32_t y, const Pixel& pixel)
{
    if (x < 0 || y < 0 || x > size.x || y > size.y)
    {
        std::cerr << "Accessing pixels of image OOB at: (" << x << ", " << y << ")\n";
        return;
    }
    size_t index = (y * size.x + x) * 4;
    pixels[index]     = pixel.r;
    pixels[index + 1] = pixel.g;
    pixels[index + 2] = pixel.b;
    pixels[index + 3] = pixel.a;
}


// ############################### LAYER ###############################
template <typename T>
Layer<T>::Layer(const std::string& name, const vec2& pos, const vec2& size, float opacity, short index)
      : name(name), pos(pos), opacity(opacity), index(index)
{
}