// Texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include "PPMWriter.hpp" // Make sure you have a Color class or struct

class Texture {
public:
    Texture(const std::string& filename);
    Color getColorAt(float u, float v) const;

private:
    std::vector<std::vector<Color>> data;
    int width, height;

    void loadPPM(const std::string& filename);
    Color interpolateColor(float u, float v) const;
};

#endif // TEXTURE_H
