#pragma once

#include "../../common/include/point.hpp"

#include <vector>
#include <string>

// virtual = 0 means that the subclass must overwrite the method
// making this an abstract class
class Primitive {
  public:
    virtual std::vector<Point> calculateCoords() const = 0;
    virtual ~Primitive(){};
};
