#pragma once

#include "shapes/primitive.hpp"

class Cylinder : public Primitive {
private:
    float radius;
    int height;
    size_t n_slices;
    size_t n_stacks;

public:
    Cylinder(int argc, char** argv);
    std::vector<Point> calculateCoords() const;
    ~Cylinder() {};
};