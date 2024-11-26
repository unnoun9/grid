#include "flip.h"

// Flip an image along the y-axis
void flip::flip_y(Raster& img) {
    int width = img.m_size.x;
    int height = img.m_size.y;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width / 2; ++x) {
            
            Pixel left_pixel = img.get_pixel(x, y);                  // Get the left pixel
            Pixel right_pixel = img.get_pixel(width - x - 1, y);     // Get the corresponding right pixel

            // Swap the pixels
            img.set_pixel(x, y, right_pixel);
            img.set_pixel(width - x - 1, y, left_pixel);
        }
    }
}

// Flip an image along the x-axis
void flip::flip_x(Raster& img) {
    int width = img.m_size.x;
    int height = img.m_size.y;

    for (int y = 0; y < height/2; y++) {
        for (int x=0; x < width; x++) {
            Pixel top_pixel = img.get_pixel(x, y);
            Pixel bottom_pixel = img.get_pixel(x, height - y -1);

            img.set_pixel(x, y, bottom_pixel);
            img.set_pixel(x, height - y -1, top_pixel);
        }
    }
}