#ifndef IMAGE_H
#define IMAGE_H

#include "json.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "PPMWriter.hpp"
#include <string>
#include "Texture.hpp"
#include <unordered_map>
#include <memory>
#include "BVHNode.hpp"
#include "BVHBuilder.hpp"
#include <iostream>
using json = nlohmann::json;

class Image {
public:
    Image(const std::string& jsonFilePath);
    void render();
    void writeToPPM(const std::string& filename) const;

   

private:
    void renderBinary();
    void renderPhong();
    Vec3 toneMap(const Vec3& color);
    Color toDisplayColor(const Vec3& hdrColor);
    Vec3 traceRay(const Ray& ray, int depth);
    Vec3 reflect(const Vec3& incident, const Vec3& normal);
    Vec3 mixColor(const Vec3& surfaceColor, const Vec3& reflectedColor, float reflectivity);
    float clamp(float lo, float hi, float v);
    std::shared_ptr<Texture> getTexture(const std::string& filename);
    Vec3 shade(const IntersectionInfo& hitInfo, const Ray& ray, int depth);
    Vec3 getSurfaceColor(const IntersectionInfo& hitInfo, const Ray& ray);
    Vec3 calculatePhongShading(const IntersectionInfo& hitInfo, const Ray& ray, const Vec3& surfaceColor);
    Ray reflect(const Ray& ray, const IntersectionInfo& hitInfo);
    Ray refract(const Ray& ray, const IntersectionInfo& hitInfo);
    float randomFloat();


    Scene scene;
    Camera cam;
    std::string renderMode;
    int nbounces;
    PPMWriter ppm;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
    std::unique_ptr<BVHNode> bvhRoot;
};

#endif // IMAGE_H
