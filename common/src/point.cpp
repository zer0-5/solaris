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

void Point::normalize() noexcept {
    auto len = sqrt(_x * _x + _y * _y + _z * _z);

    _x = _x / len;
    _y = _y / len;
    _z = _z / len;
}

Point& Point::dot(Point rhs) noexcept {
    _x *= rhs._x;
    _y *= rhs._y;
    _z *= rhs._z;

    return *this;
}

Point& Point::cross(Point rhs) noexcept {
    *this = Point::cartesian(
        _y * rhs._z - rhs._y * _z,
        _z * rhs._x - rhs._z * _x,
        _x * rhs._y - rhs._x * _y
    );

    return *this;
}

Point Point::operator+(Point rhs) const noexcept {
    return Point::cartesian(_x + rhs._x, _y + rhs._y, _z + rhs._z);
}

Point& Point::operator+=(const Point& rhs) noexcept {
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;

    return *this;
}

Point Point::operator-(Point rhs) const noexcept {
    return Point::cartesian(_x - rhs._x, _y - rhs._y, _z - rhs._z);
}

Point& Point::operator-=(const Point& rhs) noexcept {
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;

    return *this;
}

Point Point::operator*(float rhs) const noexcept {
    return Point::cartesian(rhs * _x, rhs * _y, rhs * _z);
}

Point& Point::operator*=(const float& rhs) noexcept {
    _x *= rhs;
    _y *= rhs;
    _z *= rhs;

    return *this;
}

bool Point::operator!=(Point rhs) const noexcept {
    return (_x != rhs._x || _y != rhs._y || _z != rhs._z);
}

auto operator<<(std::ostream& oss, Point const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y << ' ' << p._z;
    return oss;
}
