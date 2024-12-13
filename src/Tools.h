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

    struct Stroke
    {
        vec2 pos;
        sf::Color color;

        Stroke(vec2 pos, sf::Color color)
        : pos(pos), color(color) {}
    };

    Canvas* canvas;                                 // to access canvas variables
    i32 current_tool = NO;                          // just the index of the current tool; used to index into other variables here; only data and functionality related to this tool will be used
    Layer* layer = nullptr;                         // the layer on which the current tool acts upon
    bool is_dragging = false;
    i32 brush_size = 20;                            // for now; later this shall be modifyable from the settings panel of the current selected tool 
    std::vector<Stroke> brush_strokes;
    vec2 layer_offset;
    void (*use_current_tool[NUM_TOOLS])(Tools&);    // pointers to functions that do each tool's job

    Tools(Canvas*);
};

void no(Tools&);
void move(Tools&);
void brush(Tools&);
void eraser(Tools&);
void fill(Tools&);