#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.hpp"
#include "Vec3.hpp"
#include "Material.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Cylinder : public Shape {
public:
    Vec3 center;   // Center of one of the circular faces
    Vec3 axis;     // Vector representing the cylinder's axis direction
    float radius;  // Radius of the cylinder
    float height;  // Height of the cylinder
    Material material; // Material of the cylinder
    Vec3 normalizedAxis; // Normalized axis direction

    // Constructor updates...
    Cylinder() : radius(0), height(0), normalizedAxis(Vec3(0, 1, 0)) {}
    Cylinder(const Vec3& center, const Vec3& axis, float radius, float height, const Material& material)
        : Shape(), center(center), axis(axis), radius(radius), height(height), material(material), normalizedAxis(axis.normalize()) {}


    void fromJson(const json& j) override; // Override from Shape
    bool ray_intersection_binary(const Ray& ray) const override;
    AABB getBoundingBox() const override;

    
    // New intersection method for detailed information
    std::optional<IntersectionInfo> intersect(const Ray& ray) const override;
    std::pair<float, float> getTextureCoordinates(const Vec3& point) const override;
};

#endif // CYLINDER_H
