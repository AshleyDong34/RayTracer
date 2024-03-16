#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include <vector>
#include <memory>
#include "Shape.hpp"
#include "json.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Triangle.hpp"

using json = nlohmann::json;

std::vector<std::shared_ptr<Shape>> createShapesFromJson(const json& jsonArray);

#endif // SHAPE_FACTORY_H
