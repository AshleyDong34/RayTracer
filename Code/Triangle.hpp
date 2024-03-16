#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.hpp"
#include "Vec3.hpp"
#include "Material.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Triangle : public Shape {
public:
    Vec3 v0, v1, v2; // Vertices of the triangle
    Material material; // Material of the triangle

    Triangle() = default; // Default constructor
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Material& material)
        : v0(v0), v1(v1), v2(v2), material(material) {}

    void fromJson(const json& j) override;
    bool ray_intersection_binary(const Ray& ray) const override;
    AABB getBoundingBox() const override;

    // New intersection method for detailed information
    std::optional<IntersectionInfo> intersect(const Ray& ray) const override;
    std::pair<float, float> getTextureCoordinates(const Vec3& point) const override;
};

#endif // TRIANGLE_H
