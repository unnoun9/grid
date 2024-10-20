#include "util.hpp"
#include "Variables.h"
#include "Canvas.h"

extern Variables vars;

//..................................................................................................
Canvas::Canvas(vec2 window_size)
    : size(window_size * 0.8f), window_size(window_size), view_center(window_size/2)
{
    texture.create(window_size.x, window_size.y);

    if (!checker_shader.loadFromFile("shaders/checker.frag", sf::Shader::Fragment))
    {
        std::cerr << "Failed to load checker shader.\n";
        checker_shader_loaded = false;
    }
}

//..................................................................................................
void Canvas::draw()
{
    texture.clear();

    // update texture size when canvas window is resized
    if (window_size != (vec2)texture.getSize())
    {
        texture.create(window_size.x, window_size.y);
        texture.clear();
        view_center = window_size / 2;
        navigate();
    }

    // draw the checker pattern first
    const float square_size = 16.f;
    float adj_square_size = (16.f * zoom_factor);

    float starty = window_size.y / 2 - size.y / 2;
    float startx = window_size.x / 2 - size.x / 2;

    // try to use shader for drawing the checker pattern
    if (checker_shader_loaded)
    {
        checker_shader.setUniform("square_size", square_size);
        checker_shader.setUniform("zoom_factor", zoom_factor);
        checker_shader.setUniform("canvas_size", size);

        sf::RectangleShape checker_rect(size);
        checker_rect.setPosition(startx, starty);
        texture.draw(checker_rect, &checker_shader);
    }
    else
    {
        util::clamp(adj_square_size, 8.f, 64.f); // too small square_size lags the program;
    
        float endy = starty + size.y;
        float endx = startx + size.x;

        sf::RectangleShape rect(vec2(adj_square_size, adj_square_size));

        i32 i = 0;
        i32 j = 0;

        for (float y = starty; y < endy; y += adj_square_size)
        {
            j = 0;

            // adjust rectangle size if it exceeds canvas height
            if (y + adj_square_size > endy)
                rect.setSize(vec2(rect.getSize().x, endy - y));

            for (float x = startx; x < endx; x += adj_square_size)
            {
                if ((i + j) % 2 == 0)
                    rect.setFillColor({ 12, 12, 12 });
                else
                    rect.setFillColor({ 0, 0, 0 });

                // adjust rectangle size if it exceeds canvas width
                if (x + adj_square_size > endx)
                    rect.setSize(vec2(endx - x, rect.getSize().y));
                else
                    rect.setSize(vec2(adj_square_size, rect.getSize().y));


                rect.setPosition(x, y);
                texture.draw(rect);
                j++;
            }

            i++;
        }
    }

    // draw layers
    for (auto layer : layers)
    {
        switch (layer.type)
        {
        case Layer::RASTER:
        {
            Raster* img = (Raster*)layer.graphic;
            texture.draw(img->m_sprite);
            break;
        }
        // other cases here
        // ...
        default:
        {

            break;
        }
        }
    }
}

//..................................................................................................
std::string Canvas::default_layer_name()
{
    return std::string("layer ") + std::to_string(layers.size());
}

//..................................................................................................
void Canvas::navigate()
{
    sf::View view = texture.getView();

    // TODO: figure out a way to clamp the view center correctly and dynamically
    view.setCenter(view_center);

    // TODO: this needs to be dynamic, based on the canvas / image size or perhaps window size too
    util::clamp(vars.canvas_zoom_factor, 0.1f, 10.f);
    util::clamp(zoom_factor, 0.03f, 25.f);

    view.setSize(window_size * zoom_factor);

    texture.setView(view);    
}
