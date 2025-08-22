#include "teaImage.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

void makePPM(const char* filename, int width, int height, const teaVector3Color* pixels) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write PPM header
    out << "P3\n";
    out << width << " " << height << "\n";
    out << "255\n";

    // Write pixel data
    for (int i = 0; i < width * height; ++i) {
        out << (int)(pixels[i].r) << " " << (int)(pixels[i].g) << " " << (int)(pixels[i].b) << "\n";
    }

    out.close();
    std::cout << "PPM image saved to " << filename << "\n";
}

void make_svg(const char* filename, int width, int height) {
    std::ofstream out(filename);
    if (!out) return;

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";
    out << "width=\"" << width << "\" height=\"" << height << "\">\n";

    // Example: Draw a red circle in the center
    out << "<circle cx=\"" << width/2 << "\" cy=\"" << height/2
        << "\" r=\"" << (width < height ? width : height) / 4
        << "\" fill=\"red\" />\n";

    out << "</svg>\n";
}