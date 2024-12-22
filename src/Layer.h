#pragma once
#include <string>
#include "Raster.h"

const i32 LAYER_NAME_MAX_LENGTH = 64;

struct Canvas;

// a layer that contains various information
// graphic pointer should point to an image (pixel array) or some kind of graphic (like text, brush stroke, vector graphic, shapes, etc)
struct Layer
{
    enum Type
    {
        RASTER = 0, ADJUSTMENT, TEXT, SHAPE, SMART, FILL,
        NUM_TYPES
    };

    enum Blend_mode
    {
        NORMAL = 0, DISSOLVE,
        DARKEN, MULTIPLY, COLOR_BURN, LINEAR_BURN,
        LIGHTEN, SCREEN, COLOR_DODGE, LINEAR_DODGE,
        OVERLAY, SOFT_LIGHT, HARD_LIGHT, VIVID_LIGHT, LINEAR_LIGHT, PIN_LIGHT,
        DIFFERENCE, EXCLUSION, SUBTRACT, DIVIDE,
        HUE, SATURATION, COLOR, LUMINOSITY,
        NUM_BLEND_MODES
    };

    char name[LAYER_NAME_MAX_LENGTH] = "unspecified";   // layer's name; behaves as some ID of it
    Type type = RASTER;                                 // layer's type on the basis of which it's behaviour is determined
    vec2 pos;                                           // layer's position in the canvas (maybe not needed since the graphic may contain that (as the RASTER graphic does)?)
    float opacity = 100.f;                              // layer's opacity or state of transparency
    bool is_visible = true;                             // self-explanatory
    bool is_deleted = false;                            // self-explanatory
    Blend_mode blend = NORMAL;                          // specifies how the current layer's pixels and graphics behave w.r.t layers below it
    void* graphic = nullptr;                            // a graphic which could be a raster image, text, shape, etc
    
    Layer(const char* name, const vec2& pos, void* graphic, Type type, Blend_mode blend);
    Layer(const Layer& other);
    Layer(Layer&& other) noexcept;
    Layer& operator=(const Layer& other);
    Layer& operator=(Layer&& other) noexcept;
    ~Layer();

    // looks into the map and returns a string representation of the type or the blend mode of the current layer, depending on convert_blend
    const char* type_or_blend_to_cstr(bool convert_type=true);
};


// these two arrays are just so to convert the Layer enums to string for displaying in gui to the user
static const char* layer_types_str[(i32)Layer::NUM_TYPES] = {
    "Raster", "Adjustment", "Text", "Shape", "Smart", "Fill"
};
static const char* layer_blend_str[(i32)Layer::NUM_BLEND_MODES] = {
    "Normal", "Dissolve",
    "Darken", "Multiply", "Color Burn", "Linear Burn",
    "Lighten", "Screen", "Color Dodge", "Linear Dodge",
    "Overlay", "Soft Light", "Hard Light", "Vivid Light", "Linear Light", "Pin Light",
    "Difference", "Exclusion", "Subtract", "Divide",
    "Hue", "Saturation", "Color", "Luminosity"
};