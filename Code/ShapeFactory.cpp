#include "ShapeFactory.hpp"

std::vector<std::shared_ptr<Shape>> createShapesFromJson(const json& jsonArray) {
    std::vector<std::shared_ptr<Shape>> shapes;

    for (const auto& item : jsonArray) {
        std::string type = item["type"].get<std::string>();

        if (type == "sphere") {
            auto sphere = std::make_shared<Sphere>();
            sphere->fromJson(item);
            shapes.push_back(sphere);
        } else if (type == "cylinder") {
            auto cylinder = std::make_shared<Cylinder>();
            cylinder->fromJson(item);
            shapes.push_back(cylinder);
        } else if (type == "triangle") {
            auto triangle = std::make_shared<Triangle>();
            triangle->fromJson(item);
            shapes.push_back(triangle);
        } else {
            // Handle unknown shape type or throw an error
            throw std::runtime_error("Unknown shape type encountered: " + type);
        }
    }

    return shapes;
}
