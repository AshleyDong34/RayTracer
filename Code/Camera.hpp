#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include "json.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

class Camera {
public:
    std::string type;
    int width;
    int height;
    Vec3 position;
    Vec3 lookAt;
    Vec3 upVector;
    float fov;
    float exposure;

    // Default constructor
    Camera()
        : type("pinhole"), width(800), height(600), 
          position(0.0f, 0.0f, 0.0f), lookAt(0.0f, 0.0f, -1.0f), 
          upVector(0.0f, 1.0f, 0.0f), fov(60.0f), exposure(1.0f) {}

    // Constructor
    Camera(const std::string& t, int w, int h, const Vec3& pos, const Vec3& look, const Vec3& up, float f, float exp)
        : type(t), width(w), height(h), position(pos), lookAt(look), upVector(up), fov(f), exposure(exp) {}

    // A function to parse a JSON object into a Camera object
    static Camera parseFromJSON(const nlohmann::json& json) {
        return Camera{
            json.at("type").get<std::string>(),
            json.at("width").get<int>(),
            json.at("height").get<int>(),
            Vec3::parse(json.at("position").get<std::array<float, 3>>()),
            Vec3::parse(json.at("lookAt").get<std::array<float, 3>>()),
            Vec3::parse(json.at("upVector").get<std::array<float, 3>>()),
            json.at("fov").get<float>(),
            json.at("exposure").get<float>()
        };
    }

    Ray generateRay(float i, float j) const;
};

#endif // CAMERA_H
