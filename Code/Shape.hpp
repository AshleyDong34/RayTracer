#ifndef SHAPE_H
#define SHAPE_H

#include "json.hpp"
#include <vector>
#include <optional>
#include "Vec3.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "AABB.hpp"

using json = nlohmann::json;

struct IntersectionInfo {
    Vec3 point;
    Vec3 normal;
    Material material;
    float distance;
    std::pair<float, float> texCoords;

    // Default constructor
    IntersectionInfo() 
        : point(Vec3()), normal(Vec3()), material(Material()), distance(std::numeric_limits<float>::max()), texCoords(std::make_pair(0.0f, 0.0f)) {}

    // Parameterized constructor
    IntersectionInfo(const Vec3& p, const Vec3& n, const Material& m, float d, std::pair<float, float> tc = {0, 0})
        : point(p), normal(n), material(m), distance(d), texCoords(tc) {}
};


class Shape {
public:
    virtual ~Shape() = default;
    virtual void fromJson(const json& j) = 0;
    virtual bool ray_intersection_binary(const Ray& ray) const = 0;
    virtual AABB getBoundingBox() const = 0;

    // New method for detailed intersection information
    virtual std::optional<IntersectionInfo> intersect(const Ray& ray) const = 0;
    virtual std::pair<float, float> getTextureCoordinates(const Vec3& point) const = 0;
};

// Factory function to create shapes from JSON
std::vector<std::shared_ptr<Shape>> createShapesFromJson(const json& jsonArray);


#endif // SHAPE_H
