#pragma once

#include "vec3.hpp"

#include <array>
#include <utility>
#include <vector>

class Curve {
  private:
    const std::array<std::array<float, 4>, 4>& _geometry_matrix;
    std::vector<Vec3> _points;

    Curve(
        const std::array<std::array<float, 4>, 4>& geometry_matrix,
        std::vector<Vec3> points)
      : _geometry_matrix(geometry_matrix)
      , _points(std::move(points)) {}

  public:
    static auto catmull_rom(std::vector<Vec3>) -> Curve;
    static auto bezier(std::vector<Vec3>) -> Curve;
    auto get_position(float) const noexcept -> std::pair<Vec3, Vec3>;
    static auto patch_matrix(std::array<std::array<Vec3, 4>, 4>) noexcept
        -> std::array<std::array<Vec3, 4>, 4>;
};
