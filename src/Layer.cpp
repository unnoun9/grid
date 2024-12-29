#include "Layer.h"
#include <cstring>

//..................................................................................................
Layer::Layer(const char* name, const vec2& pos, void* graphic, Type type, Blend_mode blend)
    : type(type)
    , pos(pos)
    , blend(blend)
    , graphic(graphic)
{
    strncpy(this->name, name, LAYER_NAME_MAX_LENGTH - 1);
    this->name[LAYER_NAME_MAX_LENGTH - 1] = '\0';
}

//..................................................................................................
Layer::Layer(const Layer &other)
    : type(other.type)
    , pos(other.pos)
    , opacity(other.opacity)
    , is_visible(other.is_visible)
    , is_deleted(other.is_deleted)
    , blend(other.blend)
{
    // copy the graphic
    if (other.graphic)
    {
        if (type == RASTER)
        {
            Raster* raster = new Raster();
            Raster* raster_other = (Raster*)other.graphic;

            raster->texture = raster_other->texture;
            raster->sprite = raster_other->sprite;
            raster->sprite.setTexture(raster->texture);

            graphic = raster;
        }
    }
    else
    {
        graphic = nullptr;
    }

    // copy the name
    strncpy(this->name, other.name, LAYER_NAME_MAX_LENGTH - 1);
    this->name[LAYER_NAME_MAX_LENGTH - 1] = '\0';
}

//..................................................................................................
Layer::Layer(Layer&& other) noexcept
    : type(other.type)
    , pos(other.pos)
    , opacity(other.opacity)
    , is_visible(other.is_visible)
    , is_deleted(other.is_deleted)
    , blend(other.blend)
    , graphic(other.graphic)
{
    strncpy(name, other.name, LAYER_NAME_MAX_LENGTH - 1);
    name[LAYER_NAME_MAX_LENGTH - 1] = '\0';
    other.graphic = nullptr;
}

//..................................................................................................
Layer& Layer::operator=(const Layer &other)
{
    if (this != &other)
    {
        type = other.type;
        pos = other.pos;
        opacity = other.opacity;
        is_visible = other.is_visible;
        is_deleted = other.is_deleted;
        blend = other.blend;

        // copy the graphic
        if (graphic)
        {
            if (type == RASTER)
                delete (Raster*)graphic;
        }
        if (other.graphic)
        {
            if (type == RASTER)
            {
                Raster* raster = new Raster();
                Raster* raster_other = (Raster*)other.graphic;

                raster->texture = raster_other->texture;
                raster->sprite = raster_other->sprite;
                raster->sprite.setTexture(raster->texture);

                graphic = raster;
            }
        }
        else
        {
            graphic = nullptr;
        }

        // copy the name
        strncpy(this->name, other.name, LAYER_NAME_MAX_LENGTH - 1);
        this->name[LAYER_NAME_MAX_LENGTH - 1] = '\0';
    }

    return *this;
}

//..................................................................................................
Layer& Layer::operator=(Layer&& other) noexcept
{
    if (this != &other)
    {
        if (graphic)
            delete (Raster*)graphic;

        strncpy(name, other.name, LAYER_NAME_MAX_LENGTH - 1);
        type = other.type;
        pos = other.pos;
        opacity = other.opacity;
        is_visible = other.is_visible;
        is_deleted = other.is_deleted;
        blend = other.blend;
        graphic = other.graphic;

        other.graphic = nullptr;
    }
    return *this;
}

//..................................................................................................
Layer::~Layer()
{
    std::cout << name << "\n";
    if (graphic)
        if (type == RASTER)
            delete (Raster*)graphic;
}

//..................................................................................................
const char* Layer::type_or_blend_to_cstr(bool convert_type)
{
    // return "unspecified";
    if (convert_type)
        return layer_types_str[type];
    else
        return layer_blend_str[blend];
}