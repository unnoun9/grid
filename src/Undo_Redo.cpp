#include "Canvas.h"
#include "Undo_Redo.h"

//..................................................................................................
Edit::Edit(Type type, i32 layer, void* meta_data)
    : type(type), layer(layer), meta_data(meta_data)
{
}

//..................................................................................................
Edit::~Edit()
{
    if (meta_data)
    {
        switch (type)
        {
        case LAYER_ADD:
        case FLIPX:
        case FLIPY:
        case ROTATECW:
        case ROTATECCW: break;

        case LAYER_REMOVE: delete (Layer*)meta_data; break;
        case MOVE: delete (vec2*)meta_data; break;
        case ROTATE: delete (i32*)meta_data; break;

        case BRIGHTNESS:
        case CONTRAST:
        case GRAYSCALE:
        case INVERT:
        case SEPIA:
        case PIXELATE:
        case BOX_BLUR:
        case GAUSSIAN_BLUR:
        case SHARPEN:
        case EDGE_DETECT:
        case BRUSH:
        case ERASE:
        case FILL: delete (sf::Texture*)meta_data; break;

        default: break;
        }
    }
}

//..................................................................................................
void Undo_Redo::undo()
{
    if (undostack.empty())
        return;

    Edit e = undostack.back();
    redostack.push_back(e);
    
    if (e.type == Edit::LAYER_ADD)
    {
        canv->layers.erase(canv->layers.begin() + e.layer);
    }
    else if (e.type == Edit::LAYER_REMOVE)
    {
        Layer* layer = (Layer*)e.meta_data;
        canv->layers.insert(canv->layers.begin() + e.layer, *layer);
    }
    else if (e.type == Edit::MOVE)
    {
        vec2* prev_pos = (vec2*)e.meta_data;
        canv->layers[e.layer].pos = *prev_pos;
    }
    // undostack.pop_back();
}

//..................................................................................................
void Undo_Redo::redo()
{
    if (redostack.empty())
        return;


}