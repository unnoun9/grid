#include "util.hpp"
#include "Canvas.h"

//..................................................................................................
void Canvas::draw(sf::RenderWindow& window)
{
    // draw the checker pattern first
    i32 square_size = (i32)(16 * zoom_factor);
    util::clamp(square_size, 8, 64); // too small square_size lags the program; will clamp from 0.1 to 256 if shaders help in reducing lag
    
    float starty = window_size.y / 2 - size.y / 2;
    float startx = window_size.x / 2 - size.x / 2;
    float endy   = starty + size.y;
    float endx   = startx + size.x;

    sf::RectangleShape rect(vec2(square_size, square_size));

    i32 i = 0;
    i32 j = 0;

    for (float y = starty; y < endy; y += square_size)
    {
        j = 0;
        for (float x = startx; x < endx; x += square_size)
        {
            if ((i + j) % 2 == 0)
            {
                rect.setFillColor({ 12, 12, 12 });
            }
            else
            {
                rect.setFillColor({ 0, 0, 0 });
            }
            rect.setPosition(x, y);
            window.draw(rect);
            j++;
        }
        i++;
    }

    // draw layers
    for (auto layer : layers)
    {
        switch (layer.type)
        {
        case Layer::RASTER:
        {
            Image* img = (Image*)layer.graphic;
            window.draw(img->m_sprite);
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
void Canvas::navigate(sf::RenderWindow &window)
{
    sf::View view = window.getView();

    util::clamp(zoom_factor, 0.02f, 100.f);
    view.setSize(view_size * zoom_factor);

    // vec2 boundary = (vec2)view.getCenter() - (window_size / 2);
    // float boundaryx = view.getCenter()
    // float boundaryy

    // util::clamp(view_center.x, );
    // util::clamp(view_center.y, );

    
    // vec2 half_view_size = (view.getSize()) / 2.f;
    // vec2 half_canvas_size = size / 2;

    // util::clamp(view_center.x, half_canvas_size.x - half_view_size.x, half_canvas_size.x + half_view_size.x);
    // util::clamp(view_center.y, half_canvas_size.y - half_view_size.y, half_canvas_size.y + half_view_size.y);

    // util::clamp(view_center.x, view_center.x - window_size.x/2, view_center.x + window_size.x/2);
    // view_center.y + view_size.y > window_size.y - 250

    view.setCenter(view_center);

    window.setView(view);    
}
