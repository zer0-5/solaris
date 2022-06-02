#include "vec3.hpp"

#include <math.h>

Vec3 Vec3::cartesian(float x, float y, float z) {
    return Vec3(x, y, z);
}

Vec3 Vec3::spherical(float radius, float alpha, float beta) {
    return Vec3::cartesian(
        radius * cos(beta) * sin(alpha),
        radius * sin(beta),
        radius * cos(beta) * cos(alpha));
}

float Vec3::x() const {
    return _x;
}

float Vec3::y() const {
    return _y;
}

float Vec3::z() const {
    return _z;
}

void Vec3::set_x(float x) {
    _x = x;
}

void Vec3::set_y(float y) {
    _y = y;
}

void Vec3::set_z(float z) {
    _z = z;
}

void Vec3::sum_x(float x) {
    _x += x;
}

void Vec3::sum_y(float y) {
    _y += y;
}

void Vec3::sum_z(float z) {
    _z += z;
}

float Vec3::radius() const noexcept {
    return sqrt(_x * _x + _y * _y + _z * _z);
}

float Vec3::alpha() const noexcept {
    return atan2(_x, _z);
}

float Vec3::beta() const noexcept {
    return atan(_y / sqrt(_z*_z + _x*_x));
}

Vec3& Vec3::normalize() noexcept {
    auto len = sqrt(_x * _x + _y * _y + _z * _z);

    _x = _x / len;
    _y = _y / len;
    _z = _z / len;

    return *this;
}

Vec3& Vec3::dot(Vec3 rhs) noexcept {
    _x *= rhs._x;
    _y *= rhs._y;
    _z *= rhs._z;

    return *this;
}

Vec3& Vec3::cross(Vec3 rhs) noexcept {
    *this = Vec3::cartesian(
        _y * rhs._z - rhs._y * _z,
        _z * rhs._x - rhs._z * _x,
        _x * rhs._y - rhs._x * _y
    );

    return *this;
}

Vec3 Vec3::operator+(Vec3 rhs) const noexcept {
    return Vec3::cartesian(_x + rhs._x, _y + rhs._y, _z + rhs._z);
}

Vec3& Vec3::operator+=(const Vec3& rhs) noexcept {
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;

    return *this;
}

Vec3 Vec3::operator-(Vec3 rhs) const noexcept {
    return Vec3::cartesian(_x - rhs._x, _y - rhs._y, _z - rhs._z);
}

Vec3& Vec3::operator-=(const Vec3& rhs) noexcept {
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;

    return *this;
}

Vec3 Vec3::operator*(float rhs) const noexcept {
    return Vec3::cartesian(rhs * _x, rhs * _y, rhs * _z);
}

Vec3& Vec3::operator*=(const float& rhs) noexcept {
    _x *= rhs;
    _y *= rhs;
    _z *= rhs;

    return *this;
}

bool Vec3::operator==(const Vec3& rhs) const noexcept {
    return (_x == rhs._x || _y == rhs._y || _z == rhs._z);
}

bool Vec3::operator!=(const Vec3& rhs) const noexcept {
    return (_x != rhs._x || _y != rhs._y || _z != rhs._z);
}

auto operator<<(std::ostream& oss, Vec3 const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y << ' ' << p._z;
    return oss;
}

Vec3 operator*(float lhs, Vec3 rhs) noexcept {
    return Vec3::cartesian(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z());
}
