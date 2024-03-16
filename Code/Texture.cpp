// Texture.cpp
#include "Texture.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <iostream>

Texture::Texture(const std::string& filename) {
    loadPPM(filename);
}

void Texture::loadPPM(const std::string& filename) {

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // Read the header
    std::string line;
    std::getline(file, line);
    if (line != "P6") {
        throw std::runtime_error("Invalid PPM file: " + filename);
    }

    // Skip comments
    while(file.peek() == '#') {
        std::getline(file, line);
    }

    // Read width, height, and the max color value
    file >> width >> height;
    int max_val;
    file >> max_val;
    file.ignore(); // Skip the newline character

    // Read pixel data
    data.resize(height, std::vector<Color>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char r, g, b;
            file.read(reinterpret_cast<char*>(&r), 1);
            file.read(reinterpret_cast<char*>(&g), 1);
            file.read(reinterpret_cast<char*>(&b), 1);
            data[i][j] = Color(r, g, b);
        }
    }
}

Color Texture::getColorAt(float u, float v) const {
    return interpolateColor(u, v);
}

Color Texture::interpolateColor(float u, float v) const {
    // Wrap u and v if they're outside the [0, 1] range
    u = u - floor(u);
    v = v - floor(v);



    float x = u * (width - 1);
    float y = v * (height - 1);
    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);
    int x2 = (x1 + 1) < width ? x1 + 1 : x1;
    int y2 = (y1 + 1) < height ? y1 + 1 : y1;

    Color c1 = data[y1][x1];
    Color c2 = data[y1][x2];
    Color c3 = data[y2][x1];
    Color c4 = data[y2][x2];

    float fx = x - x1;
    float fy = y - y1;

    return Color(
        static_cast<unsigned char>((c1.r * (1 - fx) + c2.r * fx) * (1 - fy) + (c3.r * (1 - fx) + c4.r * fx) * fy),
        static_cast<unsigned char>((c1.g * (1 - fx) + c2.g * fx) * (1 - fy) + (c3.g * (1 - fx) + c4.g * fx) * fy),
        static_cast<unsigned char>((c1.b * (1 - fx) + c2.b * fx) * (1 - fy) + (c3.b * (1 - fx) + c4.b * fx) * fy)
    );
}