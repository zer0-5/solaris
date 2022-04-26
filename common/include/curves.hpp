#pragma once

#include "point.hpp"

#include <array>
#include <utility>
#include <vector>

class Curve {
  private:
    const std::array<std::array<float, 4>, 4> _geometry_matrix;
    std::vector<Point> _points;

    Curve(
        const std::array<std::array<float, 4>, 4>& geometry_matrix,
        std::vector<Point> points)
      : _geometry_matrix(geometry_matrix)
      , _points(std::move(points)) {}

  public:
    static Curve catmull_ron(std::vector<Point>);
    static Curve bezier(std::vector<Point>);
    auto get_position(float) const noexcept -> std::pair<Point, Point>;
};
