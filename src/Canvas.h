#pragma once
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "Layer.h"
#include "Assets.h"
#include "Tools.h"

struct Tools;

// represents the canvas
struct Canvas
{
    std::vector<Layer> layers;                      // for now this, but later maybe implement a better place to hold layers whose m_graphic can be other things too, not only raster
    Layer* current_layer = NULL;                    // the layer that is currently selected through the layers panel; maybe use an i32 index for this instead of pointer, since std::vector<Layer> will potentially move its layers when new layers are added
    sf::RenderTexture window_texture;               // the place where everything will ultimately be drawn; replaces RenderWindow
    sf::RenderTexture texture;                      // a texture that simulates the canvas; this is the actual place where stuff if present
    bool initialized = false;                       // represents whether or not the canvas was loaded with an image for the first time or if a new blank image was created

    vec2 size;                                      // the size of the canvas which is the size of the newly created or the opened image; this size will change by the cropping tool
    vec2 start_pos;                                 // the top-left position from where the actual canvas starts
    vec2 window_size;                               // canvas window size
    vec2 view_center;                               // represents where we are in the canvas so that we can tell that to sf::View
    vec2 mouse_p;                                   // the world position inside the canvas window_texture
    float zoom_factor = 1.f;                        // represents how zoomed in we are so that we can tell that to sf::View
    float relative_zoom_factor = 1.f;               // represents the zoom factor value that represents no zoom for the current canvas size
                                                    // relative zoom factor may later be used to display the user how much % are they zoomed
    ImVec4 primary_color = ImVec4(1, 1, 1, 1);      // the "foreground" color
    ImVec4 secondary_color = ImVec4(0, 0, 0, 0);    // the "background" color
    i32 current_color = 0;                          // 0 means foreground color is selected; 1 means background color is selected
    sf::RectangleShape checker_rect;                // this rect is where the checker shader does its thing
    Assets* assets;                                 // to access assets' variables
    Tools* tools;                                   // to access tool's variables
    sf::RenderTexture prev_layer_Tex;

    Canvas(vec2 window_size);

    // draw checker pattern then all the layers correctly
    void draw();
    
    // just simply returns "layer X" where X is number of layers for the sake of convenience 
    const char* default_layer_name();

    // uses zoom_level and center to zoom and pan across (effectively navigating the canvas)
    void navigate();
};