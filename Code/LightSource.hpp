#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Vec3.hpp"

class LightSource {
public:
    Vec3 intensity; // Common property for all lights

    LightSource(const Vec3& inten) : intensity(inten) {}
    virtual ~LightSource() = default;

    // Virtual method to get position, to be optionally overridden by light types that have a position
    virtual Vec3 getPosition() const { return Vec3(); } // Returns a default position, can be overridden

    // Method to get intensity; all light sources have intensity, so no need for it to be virtual
    Vec3 getIntensity() const { return intensity; }
};

#endif // LIGHTSOURCE_H
