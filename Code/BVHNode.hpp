#ifndef BVHNODE_H
#define BVHNODE_H

#include "Shape.hpp"
#include "AABB.hpp"
#include <memory>
#include <vector>

class BVHNode {
public:
    AABB box;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::shared_ptr<Shape> object; // Object stored in the node

    // Constructors
    BVHNode() = default;
    BVHNode(const AABB& box, std::shared_ptr<Shape> object)
        : box(box), object(object) {} // Note: No need to use std::move here
    BVHNode(const AABB& box, std::unique_ptr<BVHNode> left, std::unique_ptr<BVHNode> right)
        : box(box), left(std::move(left)), right(std::move(right)) {}

    // Ray intersection method
    bool hit(const Ray& ray, float t_min, float t_max, IntersectionInfo& info) const {
        if (!box.intersect(ray, t_min, t_max)) {
            return false;
        }

        bool hit_anything = false;
        float closest_so_far = t_max;

        // If this is a leaf node, check for intersection with the object
        if (object) {
            if (auto intersection = object->intersect(ray)) {
                if (intersection->distance < closest_so_far) {
                    closest_so_far = intersection->distance;
                    info = *intersection;
                    hit_anything = true;
                }
            }
        } else {
            // Check left child
            if (left && left->hit(ray, t_min, closest_so_far, info)) {
                hit_anything = true;
                closest_so_far = info.distance;  // Update closest_so_far to the closest intersection found
            }
            // Check right child, but only if we didn't find a closer intersection in the left child
            if (right && right->hit(ray, t_min, closest_so_far, info)) {
                hit_anything = true;
                // No need to update closest_so_far since right->hit already does it when updating info
            }
        }

        return hit_anything;
        }
};  

#endif // BVHNODE_H
