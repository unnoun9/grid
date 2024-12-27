#include <stack>

#include "Variables.h"
#include "Tools.h"

extern Variables vars;

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
    use_current_tool[COLOR_SELECTION] = &select_by_color;
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
        layer_size = ((Raster*)current_layer->graphic)->data.getSize();
    // handle other layer types
    else;
    
    if (t.is_dragging)
    {
        current_layer->pos = t.canvas->mouse_p - t.layer_offset;
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
    if (!current_layer)
        return;

    // function to draw a filled circle with anti-aliasing
    auto draw_circle = [&](const Tools::Point& p, Raster* raster, vec2 layer_size)
    {
        vec2i index = p.pos - current_layer->pos;
        if (index.x < 0 || index.x >= layer_size.x || index.y < 0 || index.y >= layer_size.y)
            return;

        auto get_aa_factor = [&p](float dx, float dy, float radius)
        {
            float dist = util::sqrt(dx * dx + dy * dy);
            
            // hardness controls the anti-aliasing region (0.5 to 2.5 pixels)
            float aa_region = 2.5f;
            float offset = aa_region / 2.0f;
            
            float delta = dist - (radius - offset);
            if (delta <= 0.0f) return 1.0f;
            if (delta >= aa_region) return 0.0f;
            
            // smoother falloff curve
            float t = 1.0f - (delta / aa_region);
            return t * t;
        };

        auto set_pixel = [&](i32 x, i32 y, sf::Color color, float aa_factor)
        {
            if (x >= 0 && x < layer_size.x && y >= 0 && y < layer_size.y)
            {
                float final_opacity = (p.opacity / 100.f) * (t.brush_anti_aliasing) ? aa_factor : 1.f;
                sf::Color blended = util::blend_colors(color, raster->data.getPixel(x, y), final_opacity);
                raster->data.setPixel(x, y, blended);
            }
        };

        if (p.size == 1)
        {
            float aa_factor = get_aa_factor(index.x, index.y, 0.5f);
            set_pixel(index.x, index.y, p.color, aa_factor);
            return;
        }
        else if (p.size == 2)
        {
            float aa_factor = get_aa_factor(index.x, index.y, 1);
            set_pixel(index.x, index.y, p.color, aa_factor);

            aa_factor = get_aa_factor(index.x + 1, index.y, 1);
            set_pixel(index.x + 1, index.y, p.color, aa_factor);

            aa_factor = get_aa_factor(index.x, index.y + 1, 1);
            set_pixel(index.x, index.y + 1, p.color, aa_factor);

            aa_factor = get_aa_factor(index.x + 1, index.y + 1, 1);
            set_pixel(index.x + 1, index.y + 1, p.color, aa_factor);
            return;
        }

        float radius = p.size / 2.f;
        i32 ceil_radius = std::ceil(radius + 0.5f);
        
        for (i32 y = -ceil_radius; y <= ceil_radius; ++y)
        {
            for (i32 x = -ceil_radius; x <= ceil_radius; ++x)
            {
                float aa_factor = get_aa_factor(x, y, radius);
                if (aa_factor > 0)
                {
                    set_pixel(index.x + x, index.y + y, p.color, aa_factor);
                }
            }
        }
    };

    vec2 layer_size;
    Raster* raster = nullptr;
    if (current_layer->type == Layer::RASTER)
    {
        raster = (Raster*)current_layer->graphic;
        layer_size = raster->data.getSize();
    }
    else
    {
        std::cerr << "Brush tool used on a non-raster layer\n";
        return;
    }

    static vec2 prev_mouse_p = t.canvas->mouse_p;
    static bool was_drawing = false;

    sf::FloatRect bounds(current_layer->pos, layer_size);
    if (vars.mouse_l_held && bounds.contains(t.canvas->mouse_p))
    {
        sf::Color brush_color = t.canvas->current_color == 0
            ? sf::Color((ui32)(t.canvas->primary_color.x * 255),
                        (ui32)(t.canvas->primary_color.y * 255),
                        (ui32)(t.canvas->primary_color.z * 255))
            : sf::Color((ui32)(t.canvas->secondary_color.x * 255),
                        (ui32)(t.canvas->secondary_color.y * 255),
                        (ui32)(t.canvas->secondary_color.z * 255));

        // just clicked - draw single circle
        if (!was_drawing)
        {
            draw_circle(Tools::Point(t.canvas->mouse_p, brush_color, t.brush_size, t.brush_opacity, t.brush_hardness), raster, layer_size);
            raster->update_texture();
            was_drawing = true;
            prev_mouse_p = t.canvas->mouse_p;
            return;
        }

        // distance moved
        vec2 movement = t.canvas->mouse_p - prev_mouse_p;
        float dist = movement.magnitude();
        
        // only draw new circles if we've moved enough distance
        float spacing = t.brush_size * 0.2f; // kinda controls the stroke density
        if (dist >= spacing)
        {
            vec2 direction = movement / dist;
            i32 num_steps = std::floor(dist / spacing);
            
            // draw circles along the path with consistent spacing
            for (i32 i = 1; i <= num_steps; i++)
            {
                vec2 pos = prev_mouse_p + direction * (spacing * i);
                draw_circle(Tools::Point(pos, brush_color, t.brush_size, t.brush_opacity, t.brush_hardness), raster, layer_size);
            }

            raster->update_texture();
            prev_mouse_p = prev_mouse_p + direction * (spacing * num_steps);
        }
    }
    else
    {
        was_drawing = false;
    }
}

//..................................................................................................
//..................................................................................................
void eraser(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();
    if (!current_layer)
        return;

    // function to draw a filled circle with anti-aliasing
    auto draw_circle = [&](const Tools::Point& p, Raster* raster, vec2 layer_size)
    {
        vec2i index = p.pos - current_layer->pos;
        if (index.x < 0 || index.x >= layer_size.x || index.y < 0 || index.y >= layer_size.y)
            return;

        auto get_aa_factor = [&p](float dx, float dy, float radius)
        {
            float dist = util::sqrt(dx * dx + dy * dy);
            float aa_region = 2.5f;
            float offset = aa_region / 2.0f;
            
            float delta = dist - (radius - offset);
            if (delta <= 0.0f) return 1.0f;
            if (delta >= aa_region) return 0.0f;
            
            float t = 1.0f - (delta / aa_region);
            return t * t;
        };

        auto set_pixel = [&](i32 x, i32 y, float aa_factor)
        {
            if (x >= 0 && x < layer_size.x && y >= 0 && y < layer_size.y)
            {
                sf::Color pixel = raster->data.getPixel(x, y);
                float erase_strength = (p.opacity / 100.f) * (t.eraser_anti_aliasing) ? aa_factor : 1.f;
                pixel.a = (ui8)(pixel.a * (1.0f - erase_strength));
                raster->data.setPixel(x, y, pixel);
            }
        };

        if (p.size == 1)
        {
            set_pixel(index.x, index.y, 1.0f);
            return;
        }
        else if (p.size == 2)
        {
            set_pixel(index.x, index.y, 1.0f);
            set_pixel(index.x + 1, index.y, 1.0f);
            set_pixel(index.x, index.y + 1, 1.0f);
            set_pixel(index.x + 1, index.y + 1, 1.0f);
            return;
        }

        float radius = p.size / 2.f;
        i32 ceil_radius = std::ceil(radius + 0.5f);
        
        for (i32 y = -ceil_radius; y <= ceil_radius; ++y)
        {
            for (i32 x = -ceil_radius; x <= ceil_radius; ++x)
            {
                float aa_factor = get_aa_factor(x, y, radius);
                if (aa_factor > 0)
                {
                    set_pixel(index.x + x, index.y + y, aa_factor);
                }
            }
        }
    };

    vec2 layer_size;
    Raster* raster = nullptr;
    if (current_layer->type == Layer::RASTER)
    {
        raster = (Raster*)current_layer->graphic;
        layer_size = raster->data.getSize();
    }
    else
    {
        std::cerr << "Eraser tool used on a non-raster layer\n";
        return;
    }

    static vec2 prev_mouse_p = t.canvas->mouse_p;
    static bool was_erasing = false;

    sf::FloatRect bounds(current_layer->pos, layer_size);
    if (vars.mouse_l_held && bounds.contains(t.canvas->mouse_p))
    {
        // just clicked - erase single circle
        if (!was_erasing)
        {
            draw_circle(Tools::Point(t.canvas->mouse_p, sf::Color::Transparent, t.brush_size, t.brush_opacity, t.brush_hardness), raster, layer_size);
            raster->update_texture();
            was_erasing = true;
            prev_mouse_p = t.canvas->mouse_p;
            return;
        }

        vec2 movement = t.canvas->mouse_p - prev_mouse_p;
        float dist = movement.magnitude();
        
        float spacing = t.brush_size * 0.2f;
        if (dist >= spacing)
        {
            vec2 direction = movement / dist;
            i32 num_steps = std::floor(dist / spacing);
            
            for (i32 i = 1; i <= num_steps; i++)
            {
                vec2 pos = prev_mouse_p + direction * (spacing * i);
                draw_circle(Tools::Point(pos, sf::Color::Transparent, t.eraser_size, t.eraser_opacity, t.eraser_hardness), raster, layer_size);
            }

            raster->update_texture();
            prev_mouse_p = prev_mouse_p + direction * (spacing * num_steps);
        }
    }
    else
    {
        was_erasing = false;
    }
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
        layer_size = raster->data.getSize();
    }
    else
    {
        std::cerr << "Fill tool used on a non-raster layer\n";
        return;
    }

    sf::FloatRect bounds(current_layer->pos, layer_size);
    if (just_pressed && bounds.contains(t.canvas->mouse_p))
    {
        vec2i pos = t.canvas->mouse_p - current_layer->pos;
        sf::Color target = raster->data.getPixel(pos.x, pos.y);
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
                    sf::Color pixel = raster->data.getPixel(x, y);
                    if (util::are_colors_tolerant(pixel, target, t.fill_tolerance))
                    {
                        sf::Color blended = util::blend_colors(replacement, pixel, t.fill_opacity / 100.f);
                        raster->data.setPixel(x, y, blended);
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
                       util::are_colors_tolerant(raster->data.getPixel(left - 1, span.y), target, t.fill_tolerance))
                    left--;

                i32 right = span.right;
                while (right < layer_size.x - 1 && !visited[span.y * layer_size.x + (right + 1)] && 
                       util::are_colors_tolerant(raster->data.getPixel(right + 1, span.y), target, t.fill_tolerance))
                    right++;

                // fill current span and mark as visited
                for (i32 x = left; x <= right; x++)
                {
                    i32 index = span.y * layer_size.x + x;
                    if (!filled[index])
                    {
                        sf::Color blended = util::blend_colors(replacement, raster->data.getPixel(x, span.y), t.fill_opacity / 100.f);
                        raster->data.setPixel(x, span.y, blended);
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
                            util::are_colors_tolerant(raster->data.getPixel(x, y), target, t.fill_tolerance))
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

        raster->update_texture();
    }
}

//..................................................................................................
//..................................................................................................
sf::Color compute_average_color(Raster* raster, vec2 mouse_pos, int radius = 2)
{
    sf::Image& image_data = raster->data;
    int pixel_count = 0;
    int sum_r = 0, sum_g = 0, sum_b = 0;

    int start_x = std::max(0, (int)(mouse_pos.x - radius));
    int end_x = std::min((int)image_data.getSize().x - 1, (int)mouse_pos.x + radius);
    int start_y = std::max(0, (int)(mouse_pos.y - radius));
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

            // calculating the difference, then making it normalized to be in range 0-1
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
                image_data.setPixel(x, y, sf::Color::White);                        // changing the selected pixels in the image to white
            }
        }
    }
}

void select_by_color(Tools& t)
{
    Layer* current_layer = t.canvas->current_layer();
    if (!current_layer)
        return;

    vec2 layer_size;
    if (current_layer->type == Layer::RASTER)
        layer_size = ((Raster*)current_layer->graphic)->data.getSize();
    else
        return; 

    sf::FloatRect bounds(t.canvas->start_pos, t.canvas->size);
    const float color_threshold = 0.05f;                                                 // threshold for strictness of color match

    if (vars.mouse_l_held && bounds.contains(t.canvas->mouse_p)) 
    {
        Raster* raster = (Raster*)current_layer->graphic;
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

    Raster* raster = (Raster*)current_layer->graphic;

    sf::Texture image_texture;
    image_texture.loadFromImage(raster->data);
    sf::Sprite image_sprite(image_texture);

    sf::Vector2f canvas_center = t.canvas->start_pos + t.canvas->size / 2.0f;
    sf::Vector2f modified_image_offset(image_texture.getSize().x / 2.0f, image_texture.getSize().y / 2.0f);

    image_sprite.setPosition(canvas_center - modified_image_offset);

    t.canvas->window_texture.draw(image_sprite);
}