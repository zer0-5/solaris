#pragma once

#include "shapes/primitive.hpp"

class Torus : public Primitive {
private:
    float torus_radius;
    float out_radius;
    size_t n_slices;
    size_t n_stacks;

public:
    Torus(int argc, char** argv);
    std::vector<Vertex> calculateCoords() const;
    ~Torus() {};
};