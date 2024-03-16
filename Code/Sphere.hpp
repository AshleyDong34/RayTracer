#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.hpp"
#include "Vec3.hpp"
#include "Material.hpp"
#include "json.hpp"

class Sphere : public Shape {
public:
    Vec3 center;
    float radius;
    Material material;

    Sphere() = default;
    Sphere(const Vec3& center, float radius, const Material& material)
        : center(center), radius(radius), material(material) {}

    void fromJson(const json& j) override;
    bool ray_intersection_binary(const Ray& ray) const override;
    AABB getBoundingBox() const override;

    // New intersection method for detailed information
    std::optional<IntersectionInfo> intersect(const Ray& ray) const override;
    std::pair<float, float> getTextureCoordinates(const Vec3& point) const override;
};

#endif // SPHERE_H
