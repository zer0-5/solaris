#pragma once

#include "shapes/primitive.hpp"

class Box : public Primitive {
private:
    float length;
    size_t n_divisions;

public:
    Box(int argc, char** argv);
    std::vector<Point> calculateCoords() const;
    ~Box() {};
};
