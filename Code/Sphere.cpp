#include "Sphere.hpp"

void Sphere::fromJson(const json& j) {
    center = Vec3::parse(j["center"]);
    radius = j["radius"].get<float>();
    if (j.find("material") != j.end()) {
        material = Material::fromJson(j["material"]);
    } else {
        material = Material::defaultMaterial();
    }
}

bool Sphere::ray_intersection_binary(const Ray& ray) const {
    Vec3 to_sphere = center - ray.origin;
    float proj_len = to_sphere.dot(ray.direction);
    float dist2 = to_sphere.dot(to_sphere) - proj_len * proj_len;
    float rad2 = radius * radius;
    // If the distance from the sphere center to the projection is greater
    // than the radius, there's no intersection.
    if (dist2 > rad2) return false;
    // Compute discriminant
    float thc = sqrt(rad2 - dist2);
    // Find the nearest intersection point along the ray
    float t0 = proj_len - thc;
    // If the nearest intersection point is behind the ray origin, check the farthest one
    if (t0 < 0) t0 = proj_len + thc;
    // If both intersection points are behind the ray, there's no intersection
    if (t0 < 0) return false;
    // If execution reaches here, it means the ray intersects the sphere
    return true;
}

std::optional<IntersectionInfo> Sphere::intersect(const Ray& ray) const {
    Vec3 to_sphere = center - ray.origin;
    float proj_len = to_sphere.dot(ray.direction);
    float dist2 = to_sphere.dot(to_sphere) - proj_len * proj_len;
    float rad2 = radius * radius;

    if (dist2 > rad2) return std::nullopt;

    float thc = sqrt(rad2 - dist2);
    float t0 = proj_len - thc;
    float t1 = proj_len + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) t0 = t1; // if t0 is negative, let's use t1 instead 
    if (t0 < 0) return std::nullopt; // both t0 and t1 are negative

    Vec3 intersectionPoint = ray.origin + ray.direction * t0;
    Vec3 normal = (intersectionPoint - center).normalize();

    std::pair<float, float> texCoords = {0, 0};
    if (material.textureFilename) {
        texCoords = getTextureCoordinates(intersectionPoint);
    }

    return IntersectionInfo(intersectionPoint, normal, material, t0, texCoords);
}

std::pair<float, float> Sphere::getTextureCoordinates(const Vec3& point) const {
    Vec3 p_normalized = (point - center).normalize();
    float u = 0.5f + std::atan2(p_normalized[2], p_normalized[0]) / (2.0f * M_PI);
    float v = 0.5f - std::asin(p_normalized[1]) / M_PI;
    return std::make_pair(u, v);
}

AABB Sphere::getBoundingBox() const {
    Vec3 min = center - Vec3(radius, radius, radius);
    Vec3 max = center + Vec3(radius, radius, radius);
    return AABB(min, max);
}