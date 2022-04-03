#pragma once

#include "point.hpp"

#include <memory>
#include <vector>

struct Color {
    float r;
    float g;
    float b;
};

class Model {
  private:
    std::shared_ptr<std::vector<Point>> _points;
    Color _color;


  public:
    Model(std::shared_ptr<std::vector<Point>> points, Color color)
      : _points(points)
      , _color(color) {}
    auto draw() const noexcept -> void;
    auto friend operator<<(std::ostream&, Model const&) -> std::ostream&;
};
