#pragma once

#include "shapes/primitive.hpp"

class Cone : public Primitive {
private:
    float radius;
    float height;
    size_t n_slices;
    size_t n_stacks;

public:
    Cone(int argc, char** argv);
    std::vector<Vertex> calculateCoords() const;
    ~Cone() {};
};
