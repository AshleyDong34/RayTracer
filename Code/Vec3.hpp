#ifndef VEC3_H
#define VEC3_H

#include <array>
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <string>

class Vec3 {
public:
    std::array<float, 3> v;

    Vec3() : v{0.0f, 0.0f, 0.0f} {}

    Vec3(float x, float y, float z) : v{x, y, z} {}

    Vec3(std::initializer_list<float> list) {
        if (list.size() != 3) {
            throw std::invalid_argument("Vec3 requires an initializer list with exactly three elements.");
        }
        std::copy(list.begin(), list.end(), v.begin());
    }

    // Vector addition
    Vec3 operator+(const Vec3& other) const {
        return Vec3(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
    }

    Vec3& operator+=(const Vec3& other) {
        v[0] += other.v[0];
        v[1] += other.v[1];
        v[2] += other.v[2];
        return *this; // Return a reference to the current object
    }

    // Vector subtraction
    Vec3 operator-(const Vec3& other) const {
        return Vec3(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
    }

    // Scalar multiplication
    Vec3 operator*(float scalar) const {
        return Vec3(v[0] * scalar, v[1] * scalar, v[2] * scalar);
    }

    Vec3 operator*(const Vec3& other) const {
        return Vec3(v[0] * other.v[0], v[1] * other.v[1], v[2] * other.v[2]);
    }

    // Negation operator
    Vec3 operator-() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }

    // Scalar division
    Vec3 operator/(float scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero in Vec3");
        return Vec3(v[0] / scalar, v[1] / scalar, v[2] / scalar);
    }

    // In Vec3.h, within the Vec3 class definition:

    Vec3& operator/=(float scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero in Vec3");
        v[0] /= scalar;
        v[1] /= scalar;
        v[2] /= scalar;
        return *this; // Return a reference to the current object
    }


    // Dot product
    float dot(const Vec3& other) const {
        return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
    }

    // Cross product
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            v[1] * other.v[2] - v[2] * other.v[1],
            v[2] * other.v[0] - v[0] * other.v[2],
            v[0] * other.v[1] - v[1] * other.v[0]);
    }

    // Length of the vector
    float length() const {
        return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }

    // Method to calculate the squared length of the vector
    float lengthSquared() const {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }

    // Make normalize() a const member function that returns a new Vec3
    Vec3 normalize() const {
        float len = length();
        // Guard against division by zero
        if (len == 0) throw std::runtime_error("Normalize called on zero vector");
        return Vec3(v[0] / len, v[1] / len, v[2] / len);
    }

    // Overloaded subscript operator for easy access
    float& operator[](int index) {
        if (index < 0 || index >= 3) {
            throw std::out_of_range("Index out of range for Vec3");
        }
        return v[index];
    }

    const float& operator[](int index) const {
        if (index < 0 || index >= 3) {
            throw std::out_of_range("Index out of range for Vec3");
        }
        return v[index];
    }

    // Method to parse a Vec3 from a JSON-like structure
    // Assuming json_value is an array-like structure with 3 elements
    static Vec3 parse(const std::array<float, 3>& json_value) {
        if (json_value.size() != 3) {
            throw std::invalid_argument("JSON array must have exactly 3 elements to represent a Vec3.");
        }
        return Vec3(json_value[0], json_value[1], json_value[2]);
    }
};

// Scalar multiplication (float * Vec3)
inline Vec3 operator*(float scalar, const Vec3& vec) {
    return vec * scalar; // Reuse the Vec3 * float operator
}

#endif // VEC3_H

