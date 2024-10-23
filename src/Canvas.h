#pragma once
#include <SFML/Graphics.hpp>
#include "Layer.h"

// represents the canvas
struct Canvas
{
    std::vector<Layer> layers;          // for now this, but later maybe implement a better place to hold layers whose m_graphic can be other things too, not only raster
    sf::RenderTexture texture;
    vec2 size;                          // the size of the canvas which is the size of the newly created or the opened image; this size will change by the cropping tool
    vec2 window_size;                   // canvas window size
    vec2 view_center;                   // represents where we are in the canvas so that we can tell that to sf::View
    float zoom_factor = 1.f;            // represents how zoomed in we are so that we can tell that to sf::View
    float relative_zoom_factor = 1.f;   // represents the zoom factor value that represents no zoom for the current canvas size
                                        // relative zoom factor may later be used to display the user how much % are they zoomed
    sf::Shader checker_shader;
    bool checker_shader_loaded = true;

    Canvas(vec2 window_size);

    // draw checker pattern then all the layers correctly
    void draw();
    
    // just simply returns "layer X" where X is number of layers for the sake of convenience 
    std::string default_layer_name();

    // uses zoom_level and center to zoom and pan across (effectively navigating the canvas)
    void navigate();
};