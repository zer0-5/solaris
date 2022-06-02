#pragma once

#include "vertex.hpp"

#include <array>
#include <string>
#include <vector>

// virtual = 0 means that the subclass must overwrite the method
// making this an abstract class
class Primitive {
  public:
    virtual std::vector<Vertex> calculateCoords() const = 0;
    void write(char const* const file_path) const;
    virtual ~Primitive(){};
};
