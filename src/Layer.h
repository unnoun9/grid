#pragma once
#include <string>
#include "vec2t.h"
#include "Image.h"

// a layer that contains various information
// graphic pointer should point to an image (pixel array) or some kind of graphic (like text, brush stroke, vector graphic, shapes, etc)
struct Layer
{
    enum Layer_type
    {
        EMPTY = -1, RASTER, ADJUSTMENT, TEXT, SHAPE, SMART, FILL
    };

    enum Blend_mode
    {
        NORMAL = 0, DISSOLVE,
        DARKEN, MULTIPLY, COLOR_BURN, LINEAR_BURN, DARKER_COLOR,
        LIGHTEN, SCREEN, COLOR_DODGE, LINEAR_DODGE, LIGHTER_DODGE,
        OVERLAY, SOFT_LIGHT, HARD_LIGHT, VIVID_LIGHT, LINEAR_LIGHT, PIN_LIGHT, HARD_MIX,
        DIFFERENCE, EXCLUSION, SUBTRACT, DIVIDE,
        HUE, SATURATION, COLOR, LUMINOSITY
    };

    std::string name;       // layer's name; behaves as some ID of it
    Layer_type type;        // layer's type on the basis of which it's behaviour is determined
    vec2 pos;               // layer's position in the canvas
    vec2 size;              // layer's size
    float opacity = 100.f;  // layer's opacity or state of transparency
    short index = 0;        // layer's z-index; specifies where in the "z-axis" the layer is in the canvas; this may be used to sort the layers in the canvas later
    Blend_mode blend;       // maybe use an enum for this?
    void* graphic;          // a graphic which could be a raster image, text, shape, etc

    Layer(const std::string& name, const vec2& pos, const vec2& size, short index, void* graphic, Layer_type type, float opacity, Blend_mode blend)
        : name(name), type(type), pos(pos), size(size), opacity(opacity), index(index), blend(blend), graphic(graphic) {}
};
