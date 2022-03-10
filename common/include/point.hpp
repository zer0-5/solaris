#pragma once

#include <iostream>

class Point {
  private:
    float _x, _y, _z;

  public:
    Point() : _x(0), _y(0), _z(0){};
    Point operator+(Point);
    Point(float, float, float);
    float x() const;
    float y() const;
    float z() const;
    void sumX(float);
    void sumY(float);
    void sumZ(float);
    auto friend operator<<(std::ostream&, Point const&) -> std::ostream&;
};
