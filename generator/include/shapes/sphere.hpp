#pragma once

#include "shapes/primitive.hpp"

class Sphere : public Primitive {
private:
    float radius;
    size_t n_slices;
    size_t n_stacks;

public:
    Sphere(int argc, char** argv);
    std::vector<Point> calculateCoords() const;
    ~Sphere() {};
};
