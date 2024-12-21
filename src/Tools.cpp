#include "Variables.h"
#include "Tools.h"
#include <queue>
#include <algorithm>

extern Variables vars;

Tools::Tools(Canvas* canvas)
    : canvas(canvas)
{
    use_current_tool[NO] = &no;
    use_current_tool[MOVE] = &move;
    use_current_tool[BRUSH] = &brush;
    use_current_tool[ERASER] = &eraser;
    use_current_tool[FILL] = &fill;
    use_current_tool[COLOR_SELECTION] = &select_by_color;
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

sf::Color compute_average_color(Raster* raster, vec2 mouse_pos, int radius = 2)
{
    sf::Image& image_data = raster->data;
    int pixel_count = 0;
    int sum_r = 0, sum_g = 0, sum_b = 0;

    int start_x = std::max(0, (int)(mouse_pos.x - radius));
    int end_x = std::min((int)image_data.getSize().x - 1, (int)mouse_pos.x + radius);
    int start_y = std::max(0, (int)mouse_pos.y - radius);
    int end_y = std::min((int)image_data.getSize().y - 1, (int)mouse_pos.y + radius);

    for (int y = start_y; y <= end_y; ++y)
    {
        for (int x = start_x; x <= end_x; ++x)
        {
            sf::Color color = image_data.getPixel(x, y);
            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
            pixel_count++;
        }
    }
    if (pixel_count > 0)
    {
        return sf::Color(sum_r / pixel_count, sum_g / pixel_count, sum_b / pixel_count);
    }

    return sf::Color::Black;                                                        // default : black if no pixel is found    
}

sf::Image create_mask(Raster* raster, const sf::Color& target_color, float threshold)
{
    sf::Image mask;
    vec2 layer_size = raster->data.getSize();
    mask.create(layer_size.x, layer_size.y, sf::Color::Transparent);

    for (unsigned y = 0; y < layer_size.y; ++y)
    {
        for (unsigned x = 0; x < layer_size.x; ++x)
        {
            sf::Color current_color = raster->data.getPixel(x, y);

            //calculating the difference, then making it normalized to be in range 0-1
            float color_diff = (std::abs(current_color.r - target_color.r) / 255.0f +
                                std::abs(current_color.g - target_color.g) / 255.0f +
                                std::abs(current_color.b - target_color.b) / 255.0f) / 3.0f;

            if (current_color.a > 0 && color_diff <= threshold)
            {
                mask.setPixel(x, y, sf::Color::White);                              // if the color is within the threshold, make it white
            }
        }
    }

    return mask;
}

void apply_mask_and_modify_image(Raster* raster, const sf::Image& mask)
{
    sf::Image& image_data = raster->data;

    for (unsigned y = 0; y < mask.getSize().y; ++y)
    {
        for (unsigned x = 0; x < mask.getSize().x; ++x)
        {
            if (mask.getPixel(x, y) == sf::Color::White)
            {                                                            
                image_data.setPixel(x, y, sf::Color::White);                        // Changing the selected pixels in the image to white
            }
        }
    }
}

void select_by_color(Tools& t)
{
    if (!t.layer)
        return;

    vec2 layer_size;
    if (t.layer->type == Layer::RASTER)
        layer_size = ((Raster*)t.layer->graphic)->data.getSize();
    else
        return; 

    sf::FloatRect bounds(t.canvas->start_pos, t.canvas->size);
    const float color_threshold = 0.05f;                                                 // threshold for strictness of color match

    if (vars.mouse_l_held && bounds.contains(t.canvas->mouse_p)) 
    {
        Raster* raster = (Raster*)t.layer->graphic;
        vec2 mouse_pos = t.canvas->mouse_p;

        sf::Vector2f canvas_center = t.canvas->start_pos + t.canvas->size / 2.0f;
        sf::Vector2f image_offset(raster->data.getSize().x / 2.0f, raster->data.getSize().y / 2.0f);
        sf::Vector2f image_top_left = canvas_center - image_offset;

        mouse_pos.x -= image_top_left.x;
        mouse_pos.y -= image_top_left.y;

        if (mouse_pos.x < 0 || mouse_pos.x >= raster->data.getSize().x ||                // check if mouse is within image bounds
            mouse_pos.y < 0 || mouse_pos.y >= raster->data.getSize().y)
        {
            return; 
        }

        sf::Color target_color = compute_average_color(raster, mouse_pos);               // taking the average color of the selected area

        sf::Image mask = create_mask(raster, target_color, color_threshold);             // creating a mask

        apply_mask_and_modify_image(raster, mask);                                       // for now, i am making the selected pixels white
    }

    Raster* raster = (Raster*)t.layer->graphic;

    sf::Texture image_texture;
    image_texture.loadFromImage(raster->data);
    sf::Sprite image_sprite(image_texture);

    sf::Vector2f canvas_center = t.canvas->start_pos + t.canvas->size / 2.0f;
    sf::Vector2f modified_image_offset(image_texture.getSize().x / 2.0f, image_texture.getSize().y / 2.0f);

    image_sprite.setPosition(canvas_center - modified_image_offset);

    t.canvas->window_texture.draw(image_sprite);
}