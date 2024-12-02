#include "util.hpp"
#include "Variables.h"
#include "Canvas.h"

extern Variables vars;

//..................................................................................................
Canvas::Canvas(vec2 window_size, sf::RenderWindow* window)
    : window_size(window_size), view_center(window_size/2), window(window)
{
    window_texture.create(window_size.x, window_size.y);

    if (!checker_shader.loadFromFile("shaders/checker.frag", sf::Shader::Fragment))
    {
        std::cerr << "Failed to load checker shader.\n";
        checker_shader_loaded = false;
    }
}

//..................................................................................................
std::string Canvas::default_layer_name()
{
    return std::string("layer ") + std::to_string(layers.size());
}

//..................................................................................................
void Canvas::draw()
{
    start_pos = (window_size - size) / 2; // update the canvas starting position

    // update sizes, positions, etc for important variables when the canvas imgui window is resized
    if (window_size != (vec2)window_texture.getSize())
    {
        vec2 prev_start = ((vec2)window_texture.getSize() - size) / 2;
        vec2 delta = start_pos - prev_start;

        // resize the texture
        window_texture.create(window_size.x, window_size.y);
        
        // updating every raster image's center to match the checker pattern's center (maybe when more layer types are added, update their positions too?)
        for (auto& layer : layers)
        {
            switch (layer.type)
            {
            case Layer::RASTER:
            {
                layer.pos += delta;
                break;
            }
            // other layer types...
            // ...
            default:
            {

                break;
            }
            }
        }
        view_center = window_size / 2; // maybe make it only reset the center when resize is drastically bigger?
        navigate();
    }

    // intermediate texture for drawing within canvas bounds
    static sf::RenderTexture canvas_draw_texture;
    if (size != (vec2)canvas_draw_texture.getSize())
    {
        canvas_draw_texture.create(size.x, size.y);
        // set a view that clips layers to the canvas bouns
        sf::View canvas_view;
        canvas_view.setSize(size);
        canvas_view.setCenter(size / 2);
        canvas_draw_texture.setView(canvas_view);
    }
    canvas_draw_texture.clear();

    // draw the checker pattern first
    const float square_size = 16.f;
    float adj_square_size = (square_size * zoom_factor);

    // try to use shader for drawing the checker pattern
    if (checker_shader_loaded && vars.use_checker_shader)
    {
        adj_square_size = util::clamp(adj_square_size, 2.f, 64.f);
        checker_shader.setUniform("sq_size", adj_square_size);
        checker_rect.setSize(size);
        checker_rect.setPosition(vec2(0, 0)); // draw relative to intermediate texture
        canvas_draw_texture.draw(checker_rect, &checker_shader);
    }
    // super slow non-shader checker drawing
    else
    {
        adj_square_size = util::clamp(adj_square_size, 8.f, 64.f); // too small square_size lags the program;
        
        sf::View v = window_texture.getView();
        sf::FloatRect view_rect(vec2(v.getCenter().x - v.getSize().x / 2, v.getCenter().y - v.getSize().y / 2), v.getSize());

        sf::RectangleShape rect(vec2(adj_square_size, adj_square_size));
        for (float y = 0; y < size.y; y += adj_square_size)
        {
            for (float x = 0; x < size.x; x += adj_square_size)
            {
                rect.setPosition(x, y);
                rect.setFillColor(((i32)(x / adj_square_size) + (i32)(y / adj_square_size)) % 2 == 0 ?
                    sf::Color(25, 25, 25) : sf::Color(0, 0, 0));
                canvas_draw_texture.draw(rect);
            }
        }
    }

    // draw layers
    for (auto& layer : layers)
    {
        if (layer.is_visible == false)
            continue;

        switch (layer.type)
        {
        case Layer::RASTER:
        {
            Raster* img = (Raster*)layer.graphic;

            img->sprite.setPosition(layer.pos - start_pos);

            sf::Color c = img->sprite.getColor();
            c.a = (ui8)(layer.opacity / 100.f * 255);
            img->sprite.setColor(c);

            canvas_draw_texture.draw(img->sprite);
            break;
        }
        // other layer types...
        // ...
        default:
        {

            break;
        }
        }
    }

    canvas_draw_texture.display();

    // draw the intermediate texture onto the main window_texture, centering it
    window_texture.clear(sf::Color::White);
    sf::Sprite canvas_sprite(canvas_draw_texture.getTexture());
    canvas_sprite.setPosition(start_pos);
    window_texture.draw(canvas_sprite);
}

//..................................................................................................
void Canvas::navigate()
{
    sf::View view = window_texture.getView();

    // this needs to be dynamic, based on the canvas / image size or perhaps window size too
    float lower_bound = (checker_shader_loaded && vars.use_checker_shader ? 0.002f : 0.1f), upper_bound = 20.f;
    vars.canvas_zoom_factor = util::clamp(vars.canvas_zoom_factor, lower_bound, upper_bound);
    zoom_factor = util::clamp(zoom_factor, lower_bound, upper_bound);

    // apply zoom
    view.setSize(window_size * zoom_factor);

    // apply "pan"
    view.setCenter(view_center); // figure out a way to clamp the view center correctly and dynamically

    window_texture.setView(view);    
}
