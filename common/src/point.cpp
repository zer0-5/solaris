#include "point.hpp"

#include <math.h>

Point::Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Point Point::operator+(Point other) {
    return Point(_x + other._x, _y + other._y, _z + other._z);
}

Point Point::spherical(float radius, float alpha, float beta) {
    return Point(
        radius * cos(beta) * sin(alpha),
        radius * sin(beta),
        radius * cos(beta) * cos(alpha));
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

auto operator<<(std::ostream& oss, Point const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y << ' ' << p._z;
    return oss;
}
