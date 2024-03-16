#ifndef PPMWRITER_H
#define PPMWRITER_H

#include <fstream>
#include <vector>
#include <string>

// Define a struct for a color with RGB components
struct Color {
    unsigned char r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(unsigned char red, unsigned char green, unsigned char blue)
        : r(red), g(green), b(blue) {}
};

class PPMWriter {
private:
    int width, height;
    std::vector<Color> pixels;

public:
    PPMWriter(int w, int h);
    void setPixel(int x, int y, const Color& color);
    bool writeToFile(const std::string& filename) const;
};

#endif // PPMWRITER_H
