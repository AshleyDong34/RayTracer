#include "Image.hpp"
#include <fstream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <iostream>

Image::Image(const std::string& jsonFilePath)
    : ppm(0, 0) {  // Initialize ppm with dummy values; will set real values after parsing JSON
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the JSON file.");
    }
    json j;
    file >> j;
    file.close();

    scene = Scene::parseFromJSON(j["scene"]);
    cam = Camera::parseFromJSON(j["camera"]);
    renderMode = j["rendermode"].get<std::string>();
    nbounces = j.value("nbounces", 0); // Default to 0 if not present
    ppm = PPMWriter(cam.width, cam.height); // Set up PPMWriter with the correct dimensions
    // Build the BVH tree
    BVHBuilder bvhBuilder;
    bvhRoot = bvhBuilder.build(scene.shapes);
}

void Image::render() {
    if (renderMode == "binary") {
        renderBinary();
    } else if (renderMode == "phong" || renderMode == "pathtracer") {
        renderPhong();
    } else {
        throw std::runtime_error("Unknown render mode.");
    }
}

void Image::renderBinary() {
     // Implement the binary render logic, which colors shapes based on a binary hit test
    for (int y = 0; y < cam.height; y++) {
        for (int x = 0; x < cam.width; x++) {
            Color color;
            bool hit = false;

            // Generate a ray from the camera to the pixel
            Ray ray = cam.generateRay(x, y);

            // Check each shape for an intersection with the ray
            for (const auto& shape : scene.shapes) {
                if (shape->ray_intersection_binary(ray)) {
                    hit = true;
                    break;
                }
            }

            color = hit ? Color(255, 0, 0) : Color(0, 0, 0);  // Red for hit, black for no hit
            ppm.setPixel(x, y, color);
        }
    }
}

void Image::renderPhong() {
    int samplesPerPixel = 4; // Number of samples per pixel for anti-aliasing
    for (int y = 0; y < cam.height; y++) {
        for (int x = 0; x < cam.width; x++) {

            
            Vec3 color(0,0,0);

            if (renderMode == "pathtracer") {
                // Apply anti-aliasing for pathtracer mode
                for (int s = 0; s < samplesPerPixel; s++) {
                    
                    float u = float(x + randomFloat());
                    float v = float(y + randomFloat());
                    Ray ray = cam.generateRay(u, v); // Generate ray for the sample
                    color += traceRay(ray, 0);
                }
                color /= samplesPerPixel; // Average the color
            } else {
                // No anti-aliasing for other modes
                Ray ray = cam.generateRay(static_cast<float>(x), static_cast<float>(y));
                color = traceRay(ray, 0);
            }
            // Apply tone mapping and convert to display color
            color = toneMap(color);
            ppm.setPixel(x, y, toDisplayColor(color));
            

            
        }
    }
}


Vec3 Image::traceRay(const Ray& ray, int depth) {


    if (depth > nbounces) {
        return Vec3(0, 0, 0); // Exceeded the recursion limit
    }
    
    IntersectionInfo hitInfo;
    if (bvhRoot->hit(ray, 0.0f, std::numeric_limits<float>::max(), hitInfo)) {
        return shade(hitInfo, ray, depth);
    } else {
        return scene.backgroundColor; // If no intersection, return the background color
    }
    
}

Vec3 Image::shade(const IntersectionInfo& hitInfo, const Ray& ray, int depth) {

    Vec3 color = Vec3(0, 0, 0); // Start with a black pixel
    //find texture or diffuse color
    Vec3 surfaceColor = getSurfaceColor(hitInfo, ray);
    // Add ambient light contribution directly to the color
    color += surfaceColor * Vec3(0.2f, 0.2f, 0.2f) ; 

    // Add diffuse and specular lighting (Phong shading)
    color += calculatePhongShading(hitInfo, ray,surfaceColor);

    // Handle reflection
    if (hitInfo.material.isreflective && depth < nbounces) {
        Ray reflectedRay = reflect(ray, hitInfo);
        Vec3 reflectedColor = traceRay(reflectedRay, depth + 1);
        color = mixColor(surfaceColor, reflectedColor, hitInfo.material.reflectivity);
    }


    // Handle refraction
    if (hitInfo.material.isrefractive && depth < nbounces) {
        Ray refractedRay = refract(ray, hitInfo);
        Vec3 refractedColor = traceRay(refractedRay, depth + 1);
        // Mix the refracted color with the color after reflection has been considered
        color = mixColor(color, refractedColor, 1.0f - hitInfo.material.reflectivity);
    }

    return color;
}

Ray Image::reflect(const Ray& incidentRay, const IntersectionInfo& hitInfo) {
    Vec3 reflectedDir = incidentRay.direction - 2 * incidentRay.direction.dot(hitInfo.normal) * hitInfo.normal;
    Vec3 reflectedOrigin = hitInfo.point + reflectedDir * 1e-4; // Slight offset to prevent self-intersection
    return Ray(reflectedOrigin, reflectedDir);
}


Ray Image::refract(const Ray& incidentRay, const IntersectionInfo& hitInfo) {
    float ior = hitInfo.material.refractiveindex;
    Vec3 n = hitInfo.normal;
    float cosi = clamp(-1.0f, 1.0f, n.dot(incidentRay.direction));
    float etai = 1, etat = ior;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n = -n; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    
    Vec3 refractedDir;
    if (k >= 0) {
        refractedDir = eta * incidentRay.direction + (eta * cosi - sqrtf(k)) * n;
    } else {
        // Handle total internal reflection
        refractedDir = reflect(incidentRay.direction, hitInfo.normal); // You need to implement the reflect method
    }

    Vec3 refractedOrigin = hitInfo.point - refractedDir * 1e-4; // Slight offset to prevent self-intersection
    return Ray(refractedOrigin, refractedDir);
}


Vec3 Image::getSurfaceColor(const IntersectionInfo& hitInfo, const Ray& ray) {
    if (hitInfo.material.textureFilename.has_value()) {
        auto texture = getTexture(hitInfo.material.textureFilename.value());
        Color textureColor = texture->getColorAt(hitInfo.texCoords.first, hitInfo.texCoords.second);
        return Vec3(textureColor.r / 255.0f, textureColor.g / 255.0f, textureColor.b / 255.0f);
    }
    return hitInfo.material.diffusecolor;
}

Vec3 Image::calculatePhongShading(const IntersectionInfo& hitInfo, const Ray& ray, const Vec3& surfaceColor) {
    Vec3 color = Vec3(0, 0, 0);
    Vec3 V = (cam.position - hitInfo.point).normalize(); // View direction

    for (const auto& light : scene.lightSources) {
        Vec3 Lm = (light->getPosition() - hitInfo.point).normalize(); // Light direction
        Vec3 N = hitInfo.normal; // Surface normal

        // Check for shadows
        Ray shadowRay(hitInfo.point + N * 1e-4, Lm);
        bool inShadow = false;
        for (const auto& occluder : scene.shapes) {
            
            auto shadowIntersect = occluder->intersect(shadowRay);
            if (shadowIntersect && shadowIntersect->distance < (light->getPosition() - hitInfo.point).length()) {
                inShadow = true;
                break;
            }
            
        }

        if (!inShadow) {
            Vec3 Rm = (2 * N * N.dot(Lm) - Lm).normalize(); // Reflection direction
            float NdotL = std::max(N.dot(Lm), 0.0f); // Dot product of N and Lm
            float RdotV = std::max(Rm.dot(V), 0.0f); // Dot product of Rm and V
            
            color += surfaceColor  * light->getIntensity() * hitInfo.material.kd * NdotL; // Diffuse term
            color += hitInfo.material.specularcolor * light->getIntensity() * hitInfo.material.ks * pow(RdotV, hitInfo.material.specularexponent); // Specular term
        }
    }
    return color;
}




Vec3 Image::mixColor(const Vec3& color1, const Vec3& color2, float mixFactor) {
    return color1 * (1 - mixFactor) + color2 * mixFactor;
}

float Image::clamp(float lo, float hi, float v) {
    return std::max(lo, std::min(hi, v));
}

Vec3 Image::reflect(const Vec3& incident, const Vec3& normal) {
    return incident - 2 * incident.dot(normal) * normal;
}


/*
normal linear tone mapping, not as nice
Vec3 Image::toneMap(const Vec3& color) {
    float scale = 1.35f; // Adjust this factor to control brightness after tone mapping
    return color * scale;
}
*/

//none linear tone mapping, with a linear scale as well, looks nicer
//tone mapping
Vec3 Image::toneMap(const Vec3& color) {
    float luminance = color[0] * 0.2126f + color[1] * 0.7152f + color[2] * 0.0722f;
    float mappedLuminance = luminance / (luminance + 1.0f);
    float scale = mappedLuminance / luminance;
    scale *= 1.8f; // Adjust this factor to increase or decrease brightness after tone mapping
    return color * scale;
}


// Convert HDR color to display color
Color Image::toDisplayColor(const Vec3& hdrColor) {
    // Directly scale the HDR values to the [0, 255] range for 8-bit color values
    return Color(
        static_cast<unsigned char>(hdrColor[0] * 255),
        static_cast<unsigned char>(hdrColor[1] * 255),
        static_cast<unsigned char>(hdrColor[2] * 255)
    );
}

void Image::writeToPPM(const std::string& filename) const {
    if (!ppm.writeToFile(filename)) {
        throw std::runtime_error("Failed to write to PPM file.");
    }
}

std::shared_ptr<Texture> Image::getTexture(const std::string& filename) {
    auto it = textureCache.find(filename);
    if (it != textureCache.end()) {
        // Texture is already loaded
        return it->second;
    } else {
        // Load texture, store in cache, and return
        auto texture = std::make_shared<Texture>(filename);
        textureCache[filename] = texture;
        return texture;
    }
}


float Image::randomFloat() {
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 0.5f;
}


