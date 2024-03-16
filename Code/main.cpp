#include "PPMWriter.hpp"
#include <iostream>
#include "Vec3.hpp"
#include "json.hpp"
#include "Camera.hpp"
#include <fstream>
#include <memory>
#include <vector>
#include "Shape.hpp"
#include "ShapeFactory.hpp"
#include "Scene.hpp"
#include "Image.hpp"



int main() {
    try {
        Image image("scene.json");
        image.render();
        image.writeToPPM("../TestSuite/output.ppm");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

