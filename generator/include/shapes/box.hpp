#pragma once

#include "shapes/primitive.hpp"

class Box : public Primitive {
private:
    float length;
    size_t n_divisions;

public:
    Box(int argc, char** argv);
    const static std::string help_message;
    std::vector<Point> calculateCoords() const;
    ~Box() {};
};