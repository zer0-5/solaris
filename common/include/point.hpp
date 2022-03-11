#pragma once

#include <iostream>

class Point {
  private:
    float _x, _y, _z;
    Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}

  public:
    static Point spherical(float, float, float);
    static Point cartesian(float, float, float);
    Point operator+(Point);
    float x() const;
    float y() const;
    float z() const;
    void set_x(float);
    void set_y(float);
    void set_z(float);
    void sum_x(float);
    void sum_y(float);
    void sum_z(float);
    auto friend operator<<(std::ostream&, Point const&) -> std::ostream&;
};
