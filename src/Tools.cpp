#include <stack>

#include "Variables.h"
#include "Tools.h"
#include "Undo_Redo.h"

extern Variables vars;
extern Undo_Redo undo_redo;

//..................................................................................................
//..................................................................................................
Tools::Tools(Canvas* canvas)
    : canvas(canvas)
{
    use_current_tool[NO] = &no;
    use_current_tool[MOVE] = &move;
    use_current_tool[BRUSH] = &brush;
    use_current_tool[ERASER] = &eraser;
    use_current_tool[FILL] = &fill;
}

//..................................................................................................
//..................................................................................................
void move(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();
    if (!current_layer)
        return;

    vec2 layer_size;
    if (current_layer->type == Layer::RASTER)
        layer_size = ((Raster*)current_layer->graphic)->texture.getSize();
    else;

    static vec2* prev_pos = nullptr;

    // when drag starts, store the previous position
    if (t.is_dragging && !prev_pos)
    {
        prev_pos = new vec2(current_layer->pos);
    }
    
    if (t.is_dragging)
    {
        current_layer->pos = t.canvas->mouse_p - t.layer_offset;
    }
    else if (prev_pos)
    {
        // if drag ends, add the move action to the undo stack
        undo_redo.undostack.push_back(Edit(Edit::MOVE, t.canvas->current_layer_index, (vec2*)new vec2(*prev_pos)));
        undo_redo.redostack.clear();

        delete prev_pos;
        prev_pos = nullptr;
    }

    sf::FloatRect bounds(current_layer->pos, layer_size);
    if (bounds.contains(t.canvas->mouse_p))
    {
        sf::RectangleShape rect(layer_size);
        rect.setPosition(current_layer->pos);
        rect.setOutlineColor(sf::Color(0, 127, 255));
        rect.setOutlineThickness(2 * t.canvas->zoom_factor);
        rect.setFillColor(sf::Color::Transparent);
        t.canvas->window_texture.draw(rect);
    }
}

//..................................................................................................
//..................................................................................................
void brush(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();

    static vec2 prev_mouse_p = t.canvas->mouse_p;
    static bool was_drawing = false;
    static i32 last_layer_index = -1;
    static sf::RenderTexture target;

    if (!current_layer || current_layer->type != Layer::RASTER)
    {
        // reset static variables when not on a valid layer
        prev_mouse_p = t.canvas->mouse_p;
        was_drawing = false;
        return;
    }

    Raster* raster = (Raster*)current_layer->graphic;
    vec2 layer_size = raster->texture.getSize();
    sf::FloatRect bounds(current_layer->pos, layer_size);
    
    // reset state if we switched layers
    if (t.canvas->current_layer_index != last_layer_index)
    {
        was_drawing = false;
        prev_mouse_p = t.canvas->mouse_p;
        last_layer_index = t.canvas->current_layer_index;
    }

    if (target.getSize().x != layer_size.x || target.getSize().y != layer_size.y)
    {
        target.create(layer_size.x, layer_size.y);
    }

    if (!vars.mouse_l_held || !bounds.contains(t.canvas->mouse_p))
    {
        was_drawing = false;
        prev_mouse_p = t.canvas->mouse_p;
        return;
    }

    sf::Color brush_color = t.canvas->current_color == 0 
        ? sf::Color((ui32)(t.canvas->primary_color.x * 255),
                   (ui32)(t.canvas->primary_color.y * 255),
                   (ui32)(t.canvas->primary_color.z * 255))
        : sf::Color((ui32)(t.canvas->secondary_color.x * 255),
                   (ui32)(t.canvas->secondary_color.y * 255),
                   (ui32)(t.canvas->secondary_color.z * 255));

    sf::Shader& shader = t.canvas->assets->get_shader("Brush");
    shader.setUniform("texture", raster->texture);
    shader.setUniform("resolution", sf::Glsl::Vec2(layer_size.x, layer_size.y));
    shader.setUniform("brush_size", (float)t.brush_size);
    shader.setUniform("brush_color", sf::Glsl::Vec4(
        brush_color.r / 255.f,
        brush_color.g / 255.f,
        brush_color.b / 255.f,
        brush_color.a / 255.f
    ));
    shader.setUniform("brush_opacity", t.brush_opacity / 100.f);
    shader.setUniform("brush_hardness", t.brush_hardness / 100.f);

    // set brush positions for stroke interpolation
    vec2 curr_pos = t.canvas->mouse_p - current_layer->pos;
    vec2 prev_pos = prev_mouse_p - current_layer->pos;
    shader.setUniform("brush_pos", sf::Glsl::Vec2(curr_pos.x, curr_pos.y));
    shader.setUniform("prev_brush_pos", sf::Glsl::Vec2(prev_pos.x, prev_pos.y));

    // draw stroke
    target.clear(sf::Color::Transparent);
    target.draw(raster->sprite);
    target.draw(raster->sprite, &shader);
    target.display();
    
    raster->texture = target.getTexture();
    raster->sprite.setTexture(raster->texture);
    
    prev_mouse_p = t.canvas->mouse_p;
}

//..................................................................................................
//..................................................................................................
void eraser(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();

    static vec2 prev_mouse_p = t.canvas->mouse_p;
    static bool was_erasing = false;
    static i32 last_layer_index = -1;
    static sf::RenderTexture target;

    if (!current_layer || current_layer->type != Layer::RASTER)
    {
        prev_mouse_p = t.canvas->mouse_p;
        was_erasing = false;
        return;
    }

    Raster* raster = (Raster*)current_layer->graphic;
    vec2 layer_size = raster->texture.getSize();
    sf::FloatRect bounds(current_layer->pos, layer_size);
    
    if (t.canvas->current_layer_index != last_layer_index)
    {
        was_erasing = false;
        prev_mouse_p = t.canvas->mouse_p;
        last_layer_index = t.canvas->current_layer_index;
    }

    if (target.getSize().x != layer_size.x || target.getSize().y != layer_size.y)
        target.create(layer_size.x, layer_size.y);

    if (!vars.mouse_l_held || !bounds.contains(t.canvas->mouse_p))
    {
        was_erasing = false;
        prev_mouse_p = t.canvas->mouse_p;
        return;
    }

    sf::Shader& shader = t.canvas->assets->get_shader("Eraser");  // Use Eraser shader instead
    shader.setUniform("texture", raster->texture);
    shader.setUniform("resolution", sf::Glsl::Vec2(layer_size.x, layer_size.y));
    shader.setUniform("brush_size", (float)t.eraser_size);
    shader.setUniform("brush_hardness", t.eraser_hardness / 100.f);
    shader.setUniform("brush_opacity", t.eraser_opacity / 100.f);  // Added eraser opacity

    vec2 curr_pos = t.canvas->mouse_p - current_layer->pos;
    vec2 prev_pos = prev_mouse_p - current_layer->pos;
    shader.setUniform("brush_pos", sf::Glsl::Vec2(curr_pos.x, curr_pos.y));
    shader.setUniform("prev_brush_pos", sf::Glsl::Vec2(prev_pos.x, prev_pos.y));

    target.clear(sf::Color::Transparent);
    target.draw(raster->sprite, &shader);
    target.display();
    
    raster->texture = target.getTexture();
    raster->sprite.setTexture(raster->texture);
    
    prev_mouse_p = t.canvas->mouse_p;
}

//..................................................................................................
//..................................................................................................
void fill(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();
    if (!current_layer)
        return;
    static bool was_held = false;
    bool just_pressed = vars.mouse_l_held && !was_held;
    was_held = vars.mouse_l_held;

    Raster* raster = nullptr;
    vec2 layer_size;
    if (current_layer->type == Layer::RASTER)
    {
        raster = (Raster*)current_layer->graphic;
        layer_size = raster->texture.getSize();
    }
    else
    {
        std::cerr << "Fill tool used on a non-raster layer\n";
        return;
    }

    sf::FloatRect bounds(current_layer->pos, layer_size);
    if (just_pressed && bounds.contains(t.canvas->mouse_p))
    {
        sf::Image raster_img = raster->texture.copyToImage();
        vec2i pos = t.canvas->mouse_p - current_layer->pos;
        sf::Color target = raster_img.getPixel(pos.x, pos.y);

        if (target.a == 0)
            return;

        sf::Color replacement = t.canvas->current_color == 0
            ? sf::Color((ui32)(t.canvas->primary_color.x * 255),
                       (ui32)(t.canvas->primary_color.y * 255),
                       (ui32)(t.canvas->primary_color.z * 255))
            : sf::Color((ui32)(t.canvas->secondary_color.x * 255),
                       (ui32)(t.canvas->secondary_color.y * 255),
                       (ui32)(t.canvas->secondary_color.z * 255));

        if (util::are_colors_tolerant(target, replacement, 0))
            return;


        // non-contiguous fill (global fill)
        if (!t.fill_contiguous)
        {
            for (i32 y = 0; y < layer_size.y; y++)
            {
                for (i32 x = 0; x < layer_size.x; x++)
                {
                    sf::Color pixel = raster_img.getPixel(x, y);
                    if (util::are_colors_tolerant(pixel, target, t.fill_tolerance))
                    {
                        sf::Color blended = util::blend_colors(replacement, pixel, t.fill_opacity / 100.f);
                        if (target.a != 0)
                            raster_img.setPixel(x, y, blended);
                    }
                }
            }
        }
        // contiguous fill (normal flood fill)
        else
        {
            struct Span { i32 y, left, right; };
            std::vector<bool> visited(layer_size.x * layer_size.y, false);
            std::vector<bool> filled(layer_size.x * layer_size.y, false);
            std::vector<Span> spans;
            spans.reserve(layer_size.y);
            spans.push_back({pos.y, pos.x, pos.x});

            while (!spans.empty())
            {
                Span span = spans.back();
                spans.pop_back();

                i32 left = span.left;
                while (left > 0 && !visited[span.y * layer_size.x + (left - 1)] && 
                       util::are_colors_tolerant(raster_img.getPixel(left - 1, span.y), target, t.fill_tolerance))
                    left--;

                i32 right = span.right;
                while (right < layer_size.x - 1 && !visited[span.y * layer_size.x + (right + 1)] && 
                       util::are_colors_tolerant(raster_img.getPixel(right + 1, span.y), target, t.fill_tolerance))
                    right++;

                // fill current span and mark as visited
                for (i32 x = left; x <= right; x++)
                {
                    i32 index = span.y * layer_size.x + x;
                    if (!filled[index])
                    {
                        sf::Color blended = util::blend_colors(replacement, raster_img.getPixel(x, span.y), t.fill_opacity / 100.f);
                        if (target.a != 0)
                            raster_img.setPixel(x, span.y, blended);
                        filled[index] = true;
                    }
                    visited[index] = true;
                }

                // check spans above and below and add them to stack if they arer valid
                auto check_line = [&](i32 y)
                {
                    if (y < 0 || y >= layer_size.y)
                        return;

                    bool in_span = false;
                    i32 start = 0;

                    for (i32 x = left; x <= right; x++)
                    {
                        if (!visited[y * layer_size.x + x] && 
                            util::are_colors_tolerant(raster_img.getPixel(x, y), target, t.fill_tolerance))
                        {
                            if (!in_span)
                            {
                                in_span = true;
                                start = x;
                            }
                        }
                        else if (in_span)
                        {
                            spans.push_back({y, start, x - 1});
                            in_span = false;
                        }
                    }
                    if (in_span)
                        spans.push_back({y, start, right});
                };

                check_line(span.y - 1);
                check_line(span.y + 1);
            }
        }

        undo_redo.undostack.push_back({ Edit::FILL, t.canvas->current_layer_index, new sf::Texture(raster->texture) });
        undo_redo.redostack.clear();
        raster->texture.update(raster_img);
    }
}