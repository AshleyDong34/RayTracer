#ifndef BVHBUILDER_H
#define BVHBUILDER_H

#include "BVHNode.hpp"
#include "Shape.hpp"
#include <memory>
#include <vector>

class BVHBuilder {
public:
    BVHBuilder() = default;

    // Build function
    std::unique_ptr<BVHNode> build(const std::vector<std::shared_ptr<Shape>>& shapes);

private:
    // Recursive splitting function
    std::unique_ptr<BVHNode> recursiveSplit(const std::vector<std::shared_ptr<Shape>>& shapes, size_t start, size_t end);
};

#endif // BVHBUILDER_H
