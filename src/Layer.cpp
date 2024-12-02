#include "Layer.h"

Layer::Layer(const std::string& name, const vec2& pos, void* graphic, Type type, Blend_mode blend)
    : name(name), type(type), pos(pos), blend(blend), graphic(graphic)
{
}

Layer::Layer(Layer && other) noexcept
    : name(std::move(other.name))
    , type(other.type)
    , pos(other.pos)
    , opacity(other.opacity)
    , is_visible(other.is_visible)
    , blend(other.blend)
    , graphic(other.graphic)
{
    other.graphic = nullptr;
}

Layer& Layer::operator=(Layer && other) noexcept
{
    if (this != &other)
    {
        if (graphic)
            delete (Raster*)graphic;

        name = std::move(other.name);
        type = other.type;
        pos = other.pos;
        opacity = other.opacity;
        is_visible = other.is_visible;
        blend = other.blend;
        graphic = other.graphic;

        other.graphic = nullptr;
    }
    return *this;
}

Layer::~Layer()
{
    std::cout << name << "\n";
    if (graphic)
        delete (Raster*)graphic;
}

const char* Layer::type_or_blend_to_cstr(bool convert_type)
{
    if (convert_type)
        return layer_types_str[type];
    else
        return layer_blend_str[blend];
}