#pragma once

#include <vector>
#include "int.h"

struct Edit
{
    enum Type
    {
        NONE,
        LAYER_ADD, LAYER_REMOVE,
        MOVE, BRUSH, ERASE, FILL,
        FLIPX, FLIPY, ROTATE, ROTATECW, ROTATECCW,
        BRIGHTNESS, CONTRAST, GRAYSCALE, INVERT, SEPIA,
        PIXELATE, BOX_BLUR, GAUSSIAN_BLUR, SHARPEN, EDGE_DETECT,
        NUM_TYPES
    };

    Type type = NONE;   // type of edit
    i32 layer = -1;     // index of the layer this edit is applied to

    // for layer_add: index of the layer
    // for layer_remove: layer
    // for move: prev position
    // for brush: layer texture
    // for erase: layer texture
    // for fill: layer texture
    // for flipx: re-flip
    // for flipy: re-flip
    // for rotate: re-rotate
    // for rotatecw: re-rotate
    // for rotateccw: re-rotate
    // for brightness: layer texture
    // for contrast: layer texture
    // for grayscale: layer texture
    // for invert: re-invert
    // for sepia: layer texture
    // for pixelate: layer texture
    // for box_blur: layer texture
    // for gaussian_blur: layer texture
    // for sharpen: layer texture
    // for edge_detect: layer texture
    void* meta_data = nullptr;

    Edit(Type type, i32 layer, void* meta_data);
    ~Edit();
};



struct Undo_Redo
{
    std::vector<Edit> undostack;
    std::vector<Edit> redostack;
    Canvas* canv = nullptr; 

    void undo();
    void redo();
};