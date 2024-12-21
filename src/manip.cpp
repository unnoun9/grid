#include "util.h"
#include "int.h"
#include "manip.h"

// ..................................................................................................
void manip::gray_scale(Raster &img)
{
    sf::Image& data = img.data;
    for (int y = 0; y < data.getSize().y; y++)
    {
        for (int x = 0; x < data.getSize().x; x++)
        {
            sf::Color pix = data.getPixel(x, y);
            i32 gray_scale_value = 0.299 * pix.r + 0.587 * pix.g + 0.144 * pix.b;
            util::clamp(gray_scale_value, 0, 255);
            data.setPixel(x, y, 
                sf::Color((ui8)gray_scale_value, (ui8)gray_scale_value, (ui8)gray_scale_value, pix.a));
        }
    }
    img.update_texture();
} 

// ..................................................................................................
void manip::color_mask(Raster& img, float r, float g, float b)
{
    sf::Image& data = img.data;
    for (int y = 0; y < data.getSize().y; y++)
    {
        for (int x = 0; x < data.getSize().x; x++)
        {
            sf::Color pix = data.getPixel(x, y);
            i32 new_r = r * pix.r;
            i32 new_g = g * pix.g;
            i32 new_b = b * pix.b;
            util::clamp(new_r, 0, 255);
            util::clamp(new_g, 0, 255);
            util::clamp(new_b, 0, 255);
            data.setPixel(x, y, sf::Color((ui8)new_r, (ui8)new_g, (ui8)new_b, pix.a));
        }
    }
    img.update_texture();
}

// ..................................................................................................
void manip::flip_horizontally(Raster& img)
{
    int width = img.data.getSize().x;
    int height = img.data.getSize().y;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width / 2; ++x)
        {
            sf::Color left_pixel = img.data.getPixel(x, y);
            sf::Color right_pixel = img.data.getPixel(width - x - 1, y);

            img.data.setPixel(x, y, right_pixel);
            img.data.setPixel(width - x - 1, y, left_pixel);
        }
    }
    img.update_texture();
}

// ..................................................................................................
void manip::flip_vertically(Raster& img)
{
    int width = img.data.getSize().x;
    int height = img.data.getSize().y;

    for (int y = 0; y < height / 2; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Color top_pixel = img.data.getPixel(x, y);
            sf::Color bottom_pixel = img.data.getPixel(x, height - y - 1);

            img.data.setPixel(x, y, bottom_pixel);
            img.data.setPixel(x, height - y - 1, top_pixel);
        }
    }
    img.update_texture();
}
