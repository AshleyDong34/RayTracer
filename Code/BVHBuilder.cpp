#include "BVHBuilder.hpp"
#include <algorithm>
#include <limits>
#include <iostream>

// Build function
std::unique_ptr<BVHNode> BVHBuilder::build(const std::vector<std::shared_ptr<Shape>>& shapes) {
    return recursiveSplit(shapes, 0, shapes.size());
}

// Recursive splitting function
std::unique_ptr<BVHNode> BVHBuilder::recursiveSplit(const std::vector<std::shared_ptr<Shape>>& shapes, size_t start, size_t end) {
    size_t length = end - start;
    if (length == 0) {
        return nullptr;
    }

    if (length == 1) {
        return std::make_unique<BVHNode>(shapes[start]->getBoundingBox(), shapes[start]);
    }

    // Create a copy of the vector segment for sorting
    std::vector<std::shared_ptr<Shape>> sortedShapes(shapes.begin() + start, shapes.begin() + end);

    // Sort shapes based on some criterion
    std::sort(sortedShapes.begin(), sortedShapes.end(), [](const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) {
        return a->getBoundingBox().centroid()[0] < b->getBoundingBox().centroid()[0];
    });

    // Split the sorted shapes into two halves
    size_t middle = length / 2;

    // Recursively build the left and right subtrees
    auto left = recursiveSplit(sortedShapes, 0, middle);
    auto right = recursiveSplit(sortedShapes, middle, length);

    // Create a bounding box that encloses both child nodes
    AABB box = AABB::surroundingBox(left->box, right->box);

    return std::make_unique<BVHNode>(box, std::move(left), std::move(right));
}
