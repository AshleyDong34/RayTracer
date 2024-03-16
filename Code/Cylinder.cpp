#include "Cylinder.hpp"
#include <iostream>

void Cylinder::fromJson(const json& j) {
    // Parsing from json
    center = Vec3::parse(j["center"]);
    axis = Vec3::parse(j["axis"]);
    radius = j["radius"].get<float>();
    height = j["height"].get<float>();
    if (j.find("material") != j.end()) {
        material = Material::fromJson(j["material"]);
    } else {
        material = Material::defaultMaterial();
    }
}

bool Cylinder::ray_intersection_binary(const Ray& ray) const {
    Vec3 axis_dir = axis.normalize(); // Ensure the axis is normalized

    Vec3 oc = ray.origin - center;
    Vec3 d = ray.direction - (axis_dir * ray.direction.dot(axis_dir));
    Vec3 e = oc - (axis_dir * oc.dot(axis_dir));
    
    float a = d.dot(d);
    float b = 2 * e.dot(d);
    float c = e.dot(e) - radius * radius;
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false; // No intersection with the infinite cylinder
    }

    float disc_sqrt = sqrt(discriminant);
    float t0 = (-b - disc_sqrt) / (2 * a);
    float t1 = (-b + disc_sqrt) / (2 * a);

    if (t0 > t1) std::swap(t0, t1);

    // First check against the sides of the cylinder
    float t_hit = t0;
    if (t_hit < 0) {
        t_hit = t1; // If t0 is negative, use t1 instead
        if (t_hit < 0) return false; // Ray does not hit the cylinder
    }

    // Compute where the hit point is on the cylinder axis
    Vec3 hit_point = ray.origin + ray.direction * t_hit;
    float d_axis = (hit_point - center).dot(axis_dir);
    
    // Check if the intersection is within the half-height of the cylinder from its center
    if (fabs(d_axis) <= height) {
        return true; // The intersection point is within the cylinder's bounds
    }

    // Check intersection with the bottom cap
    float t_plane_bottom = - (oc.dot(axis_dir) / ray.direction.dot(axis_dir));
    hit_point = ray.origin + ray.direction * t_plane_bottom;
    if (t_plane_bottom >= 0 && (hit_point - (center - axis_dir * height)).lengthSquared() <= radius * radius) {
        return true; // Intersection with the bottom cap
    }

    // Check intersection with the top cap
    float t_plane_top = ((center + axis_dir * height) - ray.origin).dot(axis_dir) / ray.direction.dot(axis_dir);
    hit_point = ray.origin + ray.direction * t_plane_top;
    if (t_plane_top >= 0 && (hit_point - (center + axis_dir * height)).lengthSquared() <= radius * radius) {
        return true; // Intersection with the top cap
    }

    return false; // No intersection with the cylinder
}

std::optional<IntersectionInfo> Cylinder::intersect(const Ray& ray) const {

    Vec3 axis_dir = axis.normalize();
    Vec3 oc = ray.origin - center;
    Vec3 d = ray.direction - (axis_dir * ray.direction.dot(axis_dir));
    Vec3 e = oc - (axis_dir * oc.dot(axis_dir));
    
    float a = d.dot(d);
    float b = 2 * e.dot(d);
    float c = e.dot(e) - radius * radius;
    
    float discriminant = b * b - 4 * a * c;
    float disc_sqrt = sqrt(discriminant);
    float t0 = (-b - disc_sqrt) / (2 * a);
    float t1 = (-b + disc_sqrt) / (2 * a);

    if (t0 > t1) std::swap(t0, t1);

    float t_hit = t0;
    if (t_hit < 0) t_hit = t1;
    if (t_hit < 0) return std::nullopt;

    Vec3 hit_point = ray.origin + ray.direction * t_hit;
    float d_axis = (hit_point - center).dot(axis_dir);

    // Intersection with the curved surface
    if (fabs(d_axis) <= height) {
        Vec3 normal = ((hit_point - center) - axis_dir * d_axis).normalize();
        auto texCoords = getTextureCoordinates(hit_point);
        return IntersectionInfo(hit_point, normal, material, t_hit, texCoords);
    }

    // Check intersection with the bottom cap
    float t_plane_bottom = -((oc + axis_dir * height).dot(axis_dir)) / ray.direction.dot(axis_dir);
    Vec3 p_bottom = ray.origin + ray.direction * t_plane_bottom;
    if (t_plane_bottom >= 0 && (p_bottom - (center - axis_dir * height)).lengthSquared() <= (radius * radius)) {
        auto texCoords = getTextureCoordinates(p_bottom);
        return IntersectionInfo(p_bottom, -axis_dir, material, t_plane_bottom,texCoords);
    }

    // Check intersection with the top cap
    float t_plane_top = ((center + axis_dir * height) - ray.origin).dot(axis_dir) / ray.direction.dot(axis_dir);
    Vec3 p_top = ray.origin + ray.direction * t_plane_top;
    if (t_plane_top >= 0 && (p_top - (center + axis_dir * height)).lengthSquared() <= (radius * radius)) {
        auto texCoords = getTextureCoordinates(p_top);
        return IntersectionInfo(p_top, axis_dir, material, t_plane_top,texCoords);
    }

    return std::nullopt;
}

std::pair<float, float> Cylinder::getTextureCoordinates(const Vec3& point) const {
    Vec3 localPoint = point - center;
    Vec3 projectedPoint = localPoint - (localPoint.dot(normalizedAxis)) * normalizedAxis; // Project on the plane perpendicular to the axis
    float u = 0.5f + (std::atan2(projectedPoint[2], projectedPoint[0]) / (2.0f * M_PI));

    // Calculate v based on whether we're on the cap or the body
    float v;
    if (std::fabs(localPoint.dot(normalizedAxis)) < height / 2) {
        // On the body
        v = (localPoint.dot(normalizedAxis) + height / 2) / height;
    } else {
        // On the cap
        v = projectedPoint.length() / radius;
    }
    return std::make_pair(u, v);
}

AABB Cylinder::getBoundingBox() const {
    // Assuming axis is normalized
    Vec3 axisOffset = (height / 2) * axis.normalize();

    Vec3 min = Vec3(
        std::min(center[0] - radius, center[0] + radius),
        std::min(center[1] - radius, center[1] + radius),
        std::min(center[2] - radius, center[2] + radius)
    ) - axisOffset;

    Vec3 max = Vec3(
        std::max(center[0] - radius, center[0] + radius),
        std::max(center[1] - radius, center[1] + radius),
        std::max(center[2] - radius, center[2] + radius)
    ) + axisOffset;

    return AABB(min, max);
}

