#include "Triangle.hpp"

void Triangle::fromJson(const json& j) {
    v0 = Vec3::parse(j["v0"]);
    v1 = Vec3::parse(j["v1"]);
    v2 = Vec3::parse(j["v2"]);
    if (j.find("material") != j.end()) {
        material = Material::fromJson(j["material"]);
    } else {
        material = Material::defaultMaterial();
    }

}

bool Triangle::ray_intersection_binary(const Ray& ray) const {
    const float EPSILON = 0.0000001f;
    Vec3 edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = ray.direction.cross(edge2);
    a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON)
        return false; // This ray is parallel to this triangle.

    f = 1.0 / a;
    s = ray.origin - v0;
    u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return false;

    q = s.cross(edge1);
    v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);

    if (t > EPSILON) // Ray intersection
    {
        // outIntersectionPoint = ray.origin + ray.direction * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

std::optional<IntersectionInfo> Triangle::intersect(const Ray& ray) const {
    const float EPSILON = 0.0000001f;
    Vec3 edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = ray.direction.cross(edge2);
    a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON)
        return std::nullopt; // This ray is parallel to this triangle.

    f = 1.0 / a;
    s = ray.origin - v0;
    u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return std::nullopt;

    q = s.cross(edge1);
    v = f * ray.direction.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return std::nullopt;

    float t = f * edge2.dot(q);
    if (t > EPSILON) {
        Vec3 intersectionPoint = ray.origin + ray.direction * t;
        Vec3 normal = edge1.cross(edge2).normalize();

        auto texCoords = getTextureCoordinates(intersectionPoint);

        return IntersectionInfo(intersectionPoint, normal, material, t, texCoords);
    }

    return std::nullopt;
}

std::pair<float, float> Triangle::getTextureCoordinates(const Vec3& intersectionPoint) const {
    // Calculate barycentric coordinates for the intersection point
    float areaTotal = (v1 - v0).cross(v2 - v0).length();
    float area0 = (v1 - intersectionPoint).cross(v2 - intersectionPoint).length();
    float area1 = (v2 - intersectionPoint).cross(v0 - intersectionPoint).length();
    float u = area0 / areaTotal;
    float v = area1 / areaTotal;
    // Assuming a simple mapping where v0 maps to (0,0), v1 maps to (1,0), and v2 maps to (0,1)
    float interpolatedU = u * 0.0f + v * 1.0f + (1.0f - u - v) * 0.0f;
    float interpolatedV = u * 0.0f + v * 0.0f + (1.0f - u - v) * 1.0f;
    return std::make_pair(interpolatedU, interpolatedV);
}

AABB Triangle::getBoundingBox() const {
    const float EPSILON = 1e-4f; // Small epsilon value

    Vec3 min = Vec3(
        std::min({v0[0], v1[0], v2[0]}),
        std::min({v0[1], v1[1], v2[1]}),
        std::min({v0[2], v1[2], v2[2]})
    ) - Vec3(EPSILON, EPSILON, EPSILON); // Expand the bounding box slightly

    Vec3 max = Vec3(
        std::max({v0[0], v1[0], v2[0]}),
        std::max({v0[1], v1[1], v2[1]}),
        std::max({v0[2], v1[2], v2[2]})
    ) + Vec3(EPSILON, EPSILON, EPSILON); // Expand the bounding box slightly
    
    return AABB(min, max);
}