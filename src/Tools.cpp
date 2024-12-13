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

    vec2 layer_size;
    if (t.layer->type == Layer::RASTER)
        layer_size = ((Raster*)t.layer->graphic)->data.getSize();
    // handle other layer types
    else;
    
    if (t.is_dragging)
    {
        t.layer->pos = t.canvas->mouse_p - t.layer_offset;
    }

    sf::FloatRect bounds(t.layer->pos, layer_size);
    if (bounds.contains(t.canvas->mouse_p))
    {
        sf::RectangleShape rect(layer_size);
        rect.setPosition(t.layer->pos);
        rect.setOutlineColor(sf::Color(0, 127, 255));
        rect.setOutlineThickness(2 * t.canvas->zoom_factor);
        rect.setFillColor(sf::Color::Transparent);
        t.canvas->window_texture.draw(rect);
    }
}

void brush(Tools& t)
{
    if (!t.layer)
        return;

    vec2 layer_size;
    if (t.layer->type == Layer::RASTER)
        layer_size = ((Raster*)t.layer->graphic)->data.getSize();
    // handle other layer types
    else;

    static vec2 prev_mouse_p = t.canvas->mouse_p;
    sf::FloatRect bounds(t.canvas->start_pos, t.canvas->size);
    if (vars.mouse_l_held && bounds.contains(t.canvas->mouse_p))
    {
        sf::Color brush_color = t.canvas->current_color == 0 ? sf::Color((ui32)(t.canvas->primary_color.x * 255), (ui32)(t.canvas->primary_color.y * 255), (ui32)(t.canvas->primary_color.z * 255))
                            : sf::Color((ui32)(t.canvas->secondary_color.x * 255), (ui32)(t.canvas->secondary_color.y * 255), (ui32)(t.canvas->secondary_color.z * 255));
        // interpolate between last and current mouse positions
        vec2 direction = t.canvas->mouse_p - prev_mouse_p;
        float dist = direction.magnitude();
        direction = dist ? direction / dist : vec2(0, 0);
        for (i32 i = 0; i < dist; i += t.brush_size / 2)
        {
            vec2 interpolated_p = prev_mouse_p + direction * i - vec2(t.brush_size, t.brush_size);
            t.brush_strokes.emplace_back(interpolated_p, brush_color);
        }
        t.brush_strokes.emplace_back(t.canvas->mouse_p - vec2(t.brush_size, t.brush_size), brush_color);
    }
    prev_mouse_p = t.canvas->mouse_p;
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
