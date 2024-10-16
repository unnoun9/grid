#include <iostream>

#include "filters.h"
#include "util.hpp"
#include "int.h"

void filters::gray_scale(Image &img)
{
    for (int y = 0; y < img.m_size.y; y++)
    {
        for (int x = 0; x < img.m_size.x; x++)
        {
            Pixel pix = img.get_pixel(x, y);
            i32 gray_scale_value = 0.299 * pix.r + 0.587 * pix.g + 0.144 * pix.b;
            util::clamp(gray_scale_value, 0, 255);
            img.set_pixel(x, y, 
                { (ui8)gray_scale_value, (ui8)gray_scale_value, (ui8)gray_scale_value,
                (ui8)pix.a });
        }
    }
} 
