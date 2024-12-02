#pragma once
#include "Raster.h"

// code that manipulates raster images (maybe more types layer too?)
// it includes filters, transforms and other types of manipulations
// note: these are all cpu-based
namespace manip {

    void gray_scale(Raster& img);
    void color_mask(Raster& img, float r, float g, float b);
    void flip_horizontally(Raster& img);
    void flip_vertically(Raster& img);
    
}