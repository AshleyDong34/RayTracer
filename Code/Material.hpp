#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "json.hpp"  // Use the correct header file for JSON
#include "Vec3.hpp"
#include <optional>
using json = nlohmann::json;


// Material class
class Material {
public:
    float ks;  // Specular reflection constant
    float kd;  // Diffuse reflection constant
    float specularexponent;
    Vec3 diffusecolor;
    Vec3 specularcolor;
    bool isreflective;
    float reflectivity;
    bool isrefractive;
    float refractiveindex;
    std::optional<std::string> textureFilename; // Optional texture filename

    // Default constructor
    Material() = default;

    // Parameterized constructor
    Material(
        float ks,
        float kd,
        float specularexponent,
        Vec3 diffusecolor,
        Vec3 specularcolor,
        bool isreflective,
        float reflectivity,
        bool isrefractive,
        float refractiveindex,
        std::optional<std::string> textureFilename = std::nullopt
    ) : ks(ks), kd(kd), specularexponent(specularexponent),
        diffusecolor(diffusecolor), specularcolor(specularcolor),
        isreflective(isreflective), reflectivity(reflectivity),
        isrefractive(isrefractive), refractiveindex(refractiveindex),
        textureFilename(textureFilename) {}

    // Static method to parse a Material from a JSON object
    static Material fromJson(const json& j) {
        std::optional<std::string> textureFilenameOpt;
        if(j.contains("texture")) {
            textureFilenameOpt = j["texture"].get<std::string>();
        }
        return Material(
            j["ks"].get<float>(),
            j["kd"].get<float>(),
            j["specularexponent"].get<float>(),
            Vec3::parse(j["diffusecolor"]),
            Vec3::parse(j["specularcolor"]),
            j["isreflective"].get<bool>(),
            j["reflectivity"].get<float>(),
            j["isrefractive"].get<bool>(),
            j["refractiveindex"].get<float>(),
            textureFilenameOpt
        );
    }

    static Material defaultMaterial() {
        return Material(
            0.1f,   // ks: Specular reflection constant
            0.9f,   // kd: Diffuse reflection constant
            20.0f,  // specularexponent
            Vec3(0.5f, 0.5f, 0.5f), // diffusecolor: neutral grey
            Vec3(1.0f, 1.0f, 1.0f), // specularcolor: white
            false,  // isreflective: not reflective
            0.0f,   // reflectivity
            false,  // isrefractive: not refractive
            1.0f    // refractiveindex: default to 1.0 (air)
        );
    }


};

#endif // MATERIAL_H
