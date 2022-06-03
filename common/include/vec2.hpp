#pragma once

#include <iostream>

class Vec2 {
  private:
    float _x, _y;

  public:
    Vec2() = default;
    Vec2(float x, float y) : _x(x), _y(y) {}
    auto x() const noexcept -> float;
    auto y() const noexcept -> float;
    auto operator+(Vec2) const noexcept -> Vec2;
    auto operator*(float) const noexcept -> Vec2;
    auto operator*(Vec2) const noexcept -> Vec2;
    auto friend operator*(float, Vec2) noexcept -> Vec2;
    auto operator/(float) const noexcept -> Vec2;
    auto friend operator<<(std::ostream&, Vec2 const&) -> std::ostream&;
};
