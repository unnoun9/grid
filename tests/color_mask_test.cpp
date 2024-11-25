#include <iostream>
#include "../src/filters.h"
#include "int.h"
#include "image.h"

void test_color_mask() {
    // Create a Raster object
    vec2 center(100, 100);
    Raster img(center);

    // Load an image file
    bool loaded = img.loadfromfile("tests/logo.png", true);
    if (!loaded) {
        std::cerr << "Failed to load image.\n";
        return;
    }

    std::cout << "Image loaded successfully.\n";

    // Apply the color mask (reduce red and green, amplify blue)
    filters::color_mask(img, 0.5f, 0.5f, 2.0f);

    // Save the modified image
    bool saved = img.savetofile("tests/modified_color_mask.png");
    if (!saved) {
        std::cerr << "Failed to save modified image.\n";
        return;
    }

    std::cout << "Color mask applied and image saved successfully.\n";

    // Optionally, verify some pixel values
    Pixel pix = img.get_pixel(10, 10);
    std::cout << "Pixel at (10,10): R=" << (int)pix.r << " G=" << (int)pix.g
              << " B=" << (int)pix.b << " A=" << (int)pix.a << '\n';

    std::cout << "Color mask test completed.\n";
}

int main() {
    test_color_mask();
    return 0;
}
