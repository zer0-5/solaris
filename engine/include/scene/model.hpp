#pragma once

#include "point.hpp"

#include <vector>

class Model {
  private:
    std::vector<Point> _points;

  public:
    Model(std::vector<Point>&& points) : _points(std::move(points)) {}
    auto draw() const noexcept -> void;
    auto friend operator<<(std::ostream&, Model const&) -> std::ostream&;
};
