#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "LightSource.hpp"
#include "json.hpp"

using json = nlohmann::json;

class PointLight : public LightSource {
public:
    Vec3 position;

    PointLight(const Vec3& pos, const Vec3& inten) : LightSource(inten), position(pos) {}

    // Override to return the position for point lights
    Vec3 getPosition() const override { return position; }

    static std::unique_ptr<LightSource> parseFromJSON(const json& jsonObj) {
        Vec3 position = Vec3::parse(jsonObj["position"]);
        Vec3 intensity = Vec3::parse(jsonObj["intensity"]);
        return std::make_unique<PointLight>(position, intensity);
    }
};

#endif // POINTLIGHT_H
