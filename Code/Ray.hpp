#ifndef RAY_H
#define RAY_H

#include "Vec3.hpp"

class Ray {
public:
    Vec3 origin;
    Vec3 direction;

    // Constructor
    Ray(const Vec3& o, const Vec3& d)
        : origin(o), direction(d.normalize()) {}

    // Method to get the point at parameter t along the ray
    Vec3 at(float t) const {
        return origin + (direction * t);
    }
};

#endif // RAY_H
