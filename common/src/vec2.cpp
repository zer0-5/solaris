#include "vec2.hpp"

auto Vec2::x() const noexcept -> float {
    return _x;
}

auto Vec2::y() const noexcept -> float {
    return _y;
}

auto Vec2::operator+(Vec2 rhs) const noexcept -> Vec2 {
    return Vec2(_x + rhs._x, _y + rhs._y);
}

auto Vec2::operator*(float rhs) const noexcept -> Vec2 {
    return Vec2(_x * rhs, _y * rhs);
}

auto Vec2::operator*(Vec2 rhs) const noexcept -> Vec2 {
    return Vec2(_x * rhs._x, _y * rhs._y);
}

auto operator*(float lhs, Vec2 rhs) noexcept -> Vec2 {
    return Vec2(lhs * rhs._x, lhs * rhs._y);
}

auto Vec2::operator/(float rhs) const noexcept -> Vec2 {
    return Vec2(_x / rhs, _y / rhs);
}

auto operator<<(std::ostream& oss, Vec2 const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y;
    return oss;
}
