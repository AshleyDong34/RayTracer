#ifndef SCENE_H
#define SCENE_H

#include "Vec3.hpp"
#include "LightSource.hpp"
#include "Shape.hpp"
#include "json.hpp"
#include <memory>
#include <vector>
#include "PointLight.hpp"

using json = nlohmann::json;

class Scene {
public:
    Vec3 backgroundColor;
    std::vector<std::unique_ptr<LightSource>> lightSources;
    std::vector<std::shared_ptr<Shape>> shapes;

    static Scene parseFromJSON(const json& jsonObj) {
        Scene scene;

        scene.backgroundColor = Vec3::parse(jsonObj["backgroundcolor"]);

        if (jsonObj.find("lightsources") != jsonObj.end()) {

            for (const auto& lightSourceJson : jsonObj["lightsources"]) {
                std::string type = lightSourceJson["type"].get<std::string>();
                if (type == "pointlight") {
                    scene.lightSources.push_back(PointLight::parseFromJSON(lightSourceJson));
                } else {
                    // Handle other types or throw an error
                    throw std::runtime_error("Unknown light source type encountered: " + type);
                }
            }
        }
        
        scene.shapes = createShapesFromJson(jsonObj["shapes"]);

        return scene;
    }
};

#endif // SCENE_H
