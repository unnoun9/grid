#pragma once
#include "Canvas.h"

struct Canvas;

struct Filters
{
    enum Type
    {
        BRIGHTNESS, CONTRAST, GRAYSCALE, INVERT, SEPIA,
        BOX_BLUR, GAUSSIAN_BLUR, PIXELATE, EDGE, SHARPEN,
        ROTATE, RESIZE, FLIP_X, FLIP_Y,
        NUM_TYPES
    };

    std::vector<std::pair<i32, Type>> applied_filters;  // vector of layer index and applied filter type
    Canvas* canv = nullptr;
    sf::RenderTexture target;
    i32 brightness_strength = 0;                        // [-150,150] range; is added to normalized colors
    i32 contrast_strength = 0;                          // [-100,100] range
    i32 box_blur_strength = 1;                          // [1,100] range; the strength of the box blur
    i32 gauss_blur_strength = 1;                        // [1,100] range; the strength of the gaussian blur
    i32 pixelate_size = 1;                              // [1,100] range; the size of the pixelate
    

    Filters(Canvas* canv);
    void apply_filter(const std::string& filter);
};

// code that manipulates raster images (maybe more types layer too?)
// it includes filters, transforms and other types of manipulations
// note: these are all cpu-based
namespace cpu_filters {

    void gray_scale(Raster& img);
    void color_mask(Raster& img, float r, float g, float b);
    void flip_horizontally(Raster& img);
    void flip_vertically(Raster& img);
    
}