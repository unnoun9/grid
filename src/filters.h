#pragma once
#include "Image.h"

// image filters that manipulate pixels
namespace filters {

    void gray_scale(Raster& img);
    void color_mask(Raster& img, float r, float g, float b);
    
}