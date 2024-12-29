#pragma once
#include "Canvas.h"

struct Canvas;

// contains all data and functionality for every tool in the tools panel
struct Tools
{
    enum Type
    {
        NO = 0, MOVE, BRUSH, ERASER, FILL,
        NUM_TOOLS
    };

    struct Point
    {
        vec2 pos;
        sf::Color color;
        i32 size;
        float opacity;
        float hardness;

        Point(vec2 pos, sf::Color color, i32 size, float opacity, float hardness)
            : pos(pos), color(color), size(size), opacity(opacity), hardness(hardness) {}
    };

    Canvas* canvas;                                 // to access canvas variables
    i32 current_tool = NO;                          // just the index of the current tool; used to index into other variables here; only data and functionality related to this tool will be used
    void (*use_current_tool[NUM_TOOLS])(Tools&);    // pointers to functions that do each tool's job

    // the following are individual tool-related variables
    vec2 layer_offset;
    bool is_dragging = false;

    i32 brush_size = 50;
    float brush_hardness = 80.f;
    float brush_opacity = 100.f;
    bool brush_anti_aliasing = true;    // not implemented yet

    i32 eraser_size = 50;
    float eraser_hardness = 80.f;
    float eraser_opacity = 100.f;
    bool eraser_anti_aliasing = true;   // not implemented yet

    float fill_tolerance = 10;
    float fill_opacity = 100.f;
    bool fill_contiguous = true;
    bool fill_anti_aliasing = true;     // not implemented yet

    Tools(Canvas*);
};

inline void no(Tools&) {} // literally do nothing lol
void move(Tools&);
void brush(Tools&);
void eraser(Tools&);
void fill(Tools&);