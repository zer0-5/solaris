#include "point.hpp"

#include <math.h>

Point  Point::cartesian(float x, float y, float z) {
    return Point(x, y, z);
}

Point Point::spherical(float radius, float alpha, float beta) {
    return Point::cartesian(
        radius * cos(beta) * sin(alpha),
        radius * sin(beta),
        radius * cos(beta) * cos(alpha));
}

Point Point::operator+(Point other) {
    return Point::cartesian(_x + other._x, _y + other._y, _z + other._z);
}

float Point::x() const {
    return _x;
}

float Point::y() const {
    return _y;
}

float Point::z() const {
    return _z;
}

void Point::set_x(float x) {
    _x = x;
}

void Point::set_y(float y) {
    _y = y;
}

void Point::set_z(float z) {
    _z = z;
}

void Point::sum_x(float x) {
    _x += x;
}

void Point::sum_y(float y) {
    _y += y;
}

void Point::sum_z(float z) {
    _z += z;
}

float Point::radius() const noexcept {
    return sqrt(_x * _x + _y * _y + _z * _z);
}

float Point::alpha() const noexcept {
    return atan2(_x, _z);
}

float Point::beta() const noexcept {
    return atan(_y / sqrt(_z*_z + _x*_x));
}

auto operator<<(std::ostream& oss, Point const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y << ' ' << p._z;
    return oss;
}
