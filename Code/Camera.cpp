#include "Camera.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"
#include <cmath>

// Camera constructor and other member function implementations

Ray Camera::generateRay(float i, float j) const {
    Vec3 w = (position - lookAt).normalize(); // Inverting the direction for left-handed system
    Vec3 u = w.cross(upVector).normalize();    // Compute the right vector
    Vec3 v = u.cross(w);                       // Compute the actual up vector

    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    float theta = fov * M_PI / 180.0f;
    float half_height = tan(theta / 2.0f);
    float half_width = aspect_ratio * half_height;

    // Map the pixel coordinates to [-half_width, half_width] x [-half_height, half_height]
    float s = (i / width) * 2.0f * half_width - half_width;
    float t = (j / height) * 2.0f * half_height - half_height;

    Vec3 ray_direction = (u * s - v * t - w).normalize();
    return Ray(position, ray_direction);
}

