#pragma once

#include "point.hpp"

#include <vector>
#include <string>

// virtual = 0 means that the subclass must overwrite the method
// making this an abstract class
class Primitive {
  public:
    virtual std::vector<Point> calculateCoords() const = 0;
    void write(char const* const file_path) const;
    virtual ~Primitive(){};
};
