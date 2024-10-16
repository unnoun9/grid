#include "image.h"
#include <cassert>

extern vec2 window_size;


// [[[1]]], [[[2]]]


// ................................................. PIXEL .................................................
Pixel::Pixel()
{
}

Pixel::Pixel(ui8 r, ui8 g, ui8 b, ui8 a)
    : r(r), g(g), b(b), a(a)
{
}

Pixel::Pixel(ui8 r, ui8 g, ui8 b)
   : r(r), g(g), b(b), a(255)
{
}

Pixel::Pixel(const Pixel& other)
   : r(other.r), g(other.g), b(other.b), a(other.a)
{
}


// ................................................. IMAGE .................................................
bool Image::loadfromfile(const std::string& path, bool uploadtotexture)
{
    sf::Image sfimage;
    if (!sfimage.loadFromFile(path))
    {
        std::cerr << "Could not load image: " << path << std::endl;
        return false;
    }

    m_size = sfimage.getSize();
    m_pixels.resize(m_size.x * m_size.y * 4);
    memcpy(m_pixels.data(), sfimage.getPixelsPtr(), m_pixels.size());

    if (uploadtotexture) upload_to_texture(nullptr);

    return true;
}


bool Image::savetofile(const std::string& path)
{
    sf::Image sfimage;
    sfimage.create(m_size.x, m_size.y, m_pixels.data());
    if (!sfimage.saveToFile(path))
    {
        std::cerr << "Failed to save image at: " << path << std::endl;
        return false;
    }
    return true;
}


void Image::upload_to_texture(sf::Texture* texture)
{
    texture = (texture == nullptr ? &m_texture : texture);
    if (!texture->create(m_size.x, m_size.y))
    {
        std::cerr << "Failed to create texture of size: " << m_size << std::endl;
        return;
    }
    texture->update(m_pixels.data());
    m_sprite.setTexture(*texture, true);
    m_sprite.setOrigin(m_size.x / 2, m_size.y / 2);
    m_sprite.setPosition(window_size.x / 2, window_size.y / 2);
}


Pixel Image::get_pixel(int x, int y)
{
    if (x < 0 || x > m_size.x || y < 0 || y > m_size.y)
    {
        std::cerr << "Image::get_pixel() at " << vec2i(x, y) << " OOB\n";
        return Pixel();
    }
    // assert(x > 0 && x < size.x && "Image::get_pixel() x-coordinate OOB");
    // assert(y > 0 && y < size.x && "Image::get_pixel() y-coordinate OOB");

    const int index = (y * m_size.x + x) * 4;
    ui8* pixel = &m_pixels[index];
    return { pixel[0], pixel[1], pixel[2], pixel[3] };
}


void Image::set_pixel(int x, int y, const Pixel& pix)
{
    if (x < 0 || x > m_size.x || y < 0 || y > m_size.y)
    {
        std::cerr << "Image::set_pixel() at " << vec2i(x, y) << " OOB\n";
        return;
    }
    // assert(x > 0 && x < size.x && "Image::set_pixel() x-coordinate OOB");
    // assert(y > 0 && y < size.x && "Image::set_pixel() y-coordinate OOB");

    const int index = (y * m_size.x + x) * 4;
    ui8* pixel = &m_pixels[index];
    pixel[0] = pix.r;
    pixel[1] = pix.g;
    pixel[2] = pix.b;
    pixel[3] = pix.a;
}