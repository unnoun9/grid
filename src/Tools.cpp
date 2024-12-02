#include "Variables.h"
#include "Tools.h"

extern Variables vars;

Tools::Tools(Canvas* canvas)
    : canvas(canvas)
{
    use_current_tool[NO] = &no;
    use_current_tool[MOVE] = &move;
    use_current_tool[BRUSH] = &brush;
    use_current_tool[ERASER] = &eraser;
    use_current_tool[FILL] = &fill;
}

void no(Tools& t)
{
    if (!t.layer)
        return;
}

void move(Tools& t)
{
    if (!t.layer)
        return;

    vec2 mouse_p = t.canvas->window_texture.mapPixelToCoords((vec2i)(vars.mouse_pos - t.canvas->window_pos));
    vec2 layer_size;
    if (t.layer->type == Layer::RASTER)
        layer_size = ((Raster*)t.layer->graphic)->data.getSize();
    // handle other layer types
    else;
    
    if (t.is_dragging)
    {
        t.layer->pos = mouse_p - t.layer_offset;
    }

    sf::FloatRect bounds(t.layer->pos, layer_size);
    if (bounds.contains(mouse_p))
    {
        sf::RectangleShape rect(layer_size);
        rect.setPosition(t.layer->pos);
        rect.setOutlineColor(sf::Color(0, 127, 255));
        rect.setOutlineThickness(4); // maybe multiply by canvas->zoom_factor?
        rect.setFillColor(sf::Color::Transparent);
        t.canvas->window_texture.draw(rect);
    }
}

void brush(Tools& t)
{
    if (!t.layer)
        return;
}

void eraser(Tools& t)
{
    if (!t.layer)
        return;
}

void fill(Tools& t)
{
    if (!t.layer)
        return;
}
