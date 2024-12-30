#pragma once
#include "Canvas.h"

struct Canvas;
struct Undo_redo;

struct Filters
{
    Canvas* canv = nullptr;
    Undo_redo* ur = nullptr;
    sf::RenderTexture target;
    i32 rotate_angle = 0;                               // [-360,360] range; the angle to rotate the image
    i32 brightness_strength = 0;                        // [-150,150] range; is added to normalized colors
    i32 contrast_strength = 0;                          // [-100,100] range
    i32 box_blur_strength = 1;                          // [1,100] range; the strength of the box blur
    i32 gauss_blur_strength = 1;                        // [1,100] range; the strength of the gaussian blur
    i32 pixelate_size = 1;                              // [1,100] range; the size of the pixelate
    

    Filters(Canvas* canv, Undo_redo* ur);
    void apply_filter(const std::string& filter);
};