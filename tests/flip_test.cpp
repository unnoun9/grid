#include <iostream>
#include "../src/tools/transform/flip.h"
#include "../src/image.h"

void test_flip_y() {
    Raster img({100, 100});
    
    std::cout << "Loading image from 'tests/logo.png'...\n";
    if (img.loadfromfile("tests/logo.png", true)) {
        std::cout << "Image loaded successfully.\n";
    } else {
        std::cout << "Failed to load image.\n";
        return;  
    }

    std::cout << "Flipping the image along the Y-axis...\n";
    flip::flip_y(img);          
    std::cout << "Image flipped successfully.\n";

    std::cout << "Flipping the image along the X-axis...\n";
    flip::flip_x(img);          
    std::cout << "Image flipped successfully.\n";

    //img.savetofile("tests/modified_logo.png");
    
    std::cout << "Test completed. Image has been flipped along the X,Y-axis.\n";
}

int main() {
    test_flip_y();
    return 0;
}
