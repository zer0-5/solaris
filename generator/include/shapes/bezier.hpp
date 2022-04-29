#pragma once

#include "shapes/primitive.hpp"

class Bezier : public Primitive {
  private:
    std::string patch_file;
    size_t tess;

  public:
    Bezier(int argc, char** argv);
    auto calculateCoords() const -> std::vector<Point>;
    ~Bezier() {};
};
