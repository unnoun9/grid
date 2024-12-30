#pragma once

#include <vector>
#include <memory>
#include "int.h"

// for layer add: index of the layer
// for layer_remove: layer
// for move: prev position
// for brush: layer texture
// for erase: layer texture
// for fill: layer texture
// for brightness: layer texture
// for contrast: layer texture
// for grayscale: layer texture
// for sepia: layer texture
// for pixelate: layer texture
// for box_blur: layer texture
// for gaussian_blur: layer texture
// for sharpen: layer texture
// for edge_detect: layer texture
// for rotate: re-rotate
// for flipx: re-flip
// for flipy: re-flip
// for rotatecw: re-rotate
// for rotateccw: re-rotate
// for invert: re-invert

struct Filters;

struct Edit
{
    enum Type
    {
        NONE,
        LAYER_ADD, LAYER_REMOVE,
        MOVE, BRUSH, ERASE, FILL,
        FLIPX, FLIPY, ROTATE,
        BRIGHTNESS, CONTRAST, GRAYSCALE, INVERT, SEPIA,
        PIXELATE, BOX_BLUR, GAUSSIAN_BLUR, SHARPEN, EDGE_DETECT,
        NUM_TYPES
    };

    Type type = NONE;   // type of edit
    i32 layer = -1;     // index of the layer this edit is applied to

    // the meta data for the edit
    std::shared_ptr<Layer> removed_layer;
    vec2 move_prev_pos;
    std::shared_ptr<sf::Texture> tex;
    i32 anti_rotate_angle;
    
    Edit(Type type, i32 layer);
};



struct Undo_redo
{
    std::vector<Edit> undostack;
    std::vector<Edit> redostack;
    Canvas* canv = nullptr; 
    Filters* filters = nullptr;
    sf::RenderTexture target;

    void undo();
    void redo();
};