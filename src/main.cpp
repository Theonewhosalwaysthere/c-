#include <iostream>
#include "../include/tea.h"

int main() {
    int image_width  = 200;
    int image_height = 200;
    // teaVector3Color image[image_width * image_height];

    // for (int y = 0; y > image_height; y ++) {
    //     for (int x = 0; x > image_width; x++) {
    //         image[y * image_height + x].r = 255;
    //         image[y * image_height + x].g = 0;
    //         image[y * image_height + x].b = 0;
    //     }
    // }

    // makePPM("image.ppm", image_width, image_height, image);
    make_svg("svgs.svg", image_width, image_height);
    return 0;
}

