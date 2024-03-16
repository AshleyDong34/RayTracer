#include "PPMWriter.hpp"
#include <iostream>

PPMWriter::PPMWriter(int w, int h) : width(w), height(h) {
    pixels.resize(width * height);
}

void PPMWriter::setPixel(int x, int y, const Color& color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::cerr << "Attempt to set pixel out of bounds: " << x << ", " << y << std::endl;
        return;
    }
    pixels[y * width + x] = color;
}

bool PPMWriter::writeToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary | std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return false;
    }

    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (const Color& pixel : pixels) {
        ofs.write(reinterpret_cast<const char*>(&pixel.r), sizeof(pixel.r));
        ofs.write(reinterpret_cast<const char*>(&pixel.g), sizeof(pixel.g));
        ofs.write(reinterpret_cast<const char*>(&pixel.b), sizeof(pixel.b));
    }

    ofs.close();
    return true;
}
