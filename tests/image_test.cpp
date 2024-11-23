#include "../src/image.h"
#include <iostream>
#include <cassert>

//............................Test for image.cpp.................................................
//tests the two classes or structs--whatever--in the image.cpp their constructors, and if the image is loaded and saved
//as modified or not.

void test_pixel_class() {
    // Test default constructor
    Pixel p1;
    assert(p1.r == 0 && p1.g == 0 && p1.b == 0 && p1.a == 0);   //checks if p1 has (0,0,0,0) values

    // Test constructor with RGB
    Pixel p2(255, 128, 64);
    assert(p2.r == 255 && p2.g == 128 && p2.b == 64 && p2.a == 255);

    // Test constructor with RGBA
    Pixel p3(10, 20, 30, 40);
    assert(p3.r == 10 && p3.g == 20 && p3.b == 30 && p3.a == 40);

    // Test copy constructor
    Pixel p4(p3);
    assert(p4.r == 10 && p4.g == 20 && p4.b == 30 && p4.a == 40);

    std::cout << "Pixel tests passed.\n";
}

void test_raster_class() {
    vec2 center(100, 100);
    Raster raster(center);

    // Load an image file
    bool loaded = raster.loadfromfile("tests/logo.png", true);
    assert(loaded && "Failed to load image");

    // Get pixel at a valid location
    Pixel pixel = raster.get_pixel(10, 10);
    std::cout << "Pixel at (10,10): R=" << (int)pixel.r << " G=" << (int)pixel.g
              << " B=" << (int)pixel.b << " A=" << (int)pixel.a << '\n';

    // Modify a range of pixels and verify the change
    Pixel newPixel(255, 0, 0, 255); // Red
    for (int i = 10; i < 100; i++) {
        for (int j = 10; j < 100; j++) {
            raster.set_pixel(i, j, newPixel); // Set pixel to red
        }
    }
    
    // Verify that one of the modified pixels was set correctly
    Pixel modifiedPixel = raster.get_pixel(10, 10);
    assert(modifiedPixel.r == 255 && modifiedPixel.g == 0 && modifiedPixel.b == 0 && modifiedPixel.a == 255);

    // Save the modified image
    bool saved = raster.savetofile("tests/modified_image.png");
    assert(saved && "Failed to save image");

    std::cout << "Raster tests passed.\n";
}


int main() {
    try {
        test_pixel_class();
        test_raster_class();
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
