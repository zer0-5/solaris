#pragma once

#include "shapes/primitive.hpp"

class Plane : public Primitive {
  private:
    float length;
    size_t n_divisions;

  public:
    Plane(int argc, char** argv);
    std::vector<Point> calculateCoords() const;
    ~Plane() {};
};
