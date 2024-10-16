#pragma once
#include <SFML/Graphics.hpp>
#include "Layer.h"

extern vec2 window_size;


// represents the canvas
struct Canvas
{
    std::vector<Layer> layers;          // for now this, but later maybe implement a better place to hold layers whose m_graphic can be other things too, not only raster
    vec2 size = {500, 500};             // the size of the canvas which is the size of the newly created or the opened image; this size will change by the copping tool
    float zoom_factor = 1.f;            // represents how zoomed in we are so that we can tell that to sf::View
    vec2 view_size = window_size;
    vec2 view_center = (vec2)window_size/2;  // represents where we are in the canvas so that we can tell that to sf::View


    // draw checker pattern then all the layers correctly
    void draw(sf::RenderWindow& window);
    
    // just simply returns "layer X" where X is number of layers for the sake of convenience 
    std::string default_layer_name();

    // uses zoom_level and center to zoom and pan across (effectively navigating the canvas)
    void navigate(sf::RenderWindow& window);
};