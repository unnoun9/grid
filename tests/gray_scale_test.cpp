#include "../src/filters.h"
#include <iostream>
#include <cassert>

// Test for the gray_scale filter
void test_gray_scale_filter() {
    // Load the test image
    Raster img({100, 100});
    bool loaded = img.loadfromfile("tests/logo.png", true);
    assert(loaded && "Failed to load image");

    // Apply the grayscale filter
    filters::gray_scale(img);

    // Save the modified image
    bool saved = img.savetofile("tests/gray_scaled_logo.png");
    assert(saved && "Failed to save grayscale image");

    // Verify the grayscale effect by checking a few pixels
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            Pixel pix = img.get_pixel(x, y);
            assert(pix.r == pix.g && pix.g == pix.b && "Pixel is not grayscale");
        }
    }

    std::cout << "Gray scale filter test passed.\n";
}

int main() {
    try {
        test_gray_scale_filter();
    } catch (const std::exception &e) {
        std::cerr << "Test failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
