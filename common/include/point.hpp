#pragma once

#include <iostream>

class Point {
  private:
    float _x, _y, _z;

  public:
    Point() : _x(0), _y(0), _z(0) {}
    static Point spherical(float, float, float);
    Point operator+(Point);
    Point(float, float, float);
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
