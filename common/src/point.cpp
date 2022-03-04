#include "point.hpp"

Point::Point(float x, float y, float z): _x(x), _y(y), _z(z) {}

Point Point::operator+(Point other) {
    return Point(_x + other._x, _y + other._y, _z + other._z);
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

void Point::sumX(float x) {
    _x += x;
}

void Point::sumY(float y) {
    _y += y;
}

void Point::sumZ(float z) {
    _z += z;
}
