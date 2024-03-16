#ifndef AABB_H
#define AABB_H

#include "Ray.hpp"
#include "Vec3.hpp"
#include <algorithm>
#include <vector>

class AABB {
public:
    Vec3 min; // Minimum corner of the bounding box
    Vec3 max; // Maximum corner of the bounding box

    // Constructor
    AABB(const Vec3& min, const Vec3& max) : min(min), max(max) {}

    // Method to calculate the centroid of the bounding box
    Vec3 centroid() const {
        return 0.5f * min + 0.5f * max;
    }

    // Method to check intersection with a ray within a range
    bool intersect(const Ray& ray, float t_min, float t_max) const {
        for (int i = 0; i < 3; i++) {
            float invD = 1.0f / ray.direction[i];
            float t0 = (min[i] - ray.origin[i]) * invD;
            float t1 = (max[i] - ray.origin[i]) * invD;

            if (invD < 0.0f) {
                std::swap(t0, t1);
            }

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;

            if (t_max <= t_min) {
                return false;
            }
        }
        return true;
    }

    static AABB surroundingBox(const AABB& box0, const AABB& box1) {
        Vec3 small(fmin(box0.min[0], box1.min[0]),
                   fmin(box0.min[1], box1.min[1]),
                   fmin(box0.min[2], box1.min[2]));
    
        Vec3 big(fmax(box0.max[0], box1.max[0]),
                 fmax(box0.max[1], box1.max[1]),
                 fmax(box0.max[2], box1.max[2]));
    
        return AABB(small, big);
    }
    
};

#endif // AABB_H
