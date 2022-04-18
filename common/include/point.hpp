#pragma once

#include <iostream>

class Point {
  private:
    float _x, _y, _z;
    Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}

  public:
    static Point cartesian(float, float, float);
    static Point spherical(float, float, float);
    float x() const;
    float y() const;
    float z() const;
    void set_x(float);
    void set_y(float);
    void set_z(float);
    void sum_x(float);
    void sum_y(float);
    void sum_z(float);
    float radius() const noexcept;
    float alpha() const noexcept;
    float beta() const noexcept;
    void sum_alpha(float);
    void sum_beta(float);
    void normalize() noexcept;
    Point operator+(Point) const noexcept;
    Point& operator+=(const Point&) noexcept;
    Point operator-(Point) const noexcept;
    Point& operator-=(const Point&) noexcept;
    Point operator*(float) const noexcept;
    Point& operator*=(const float&) noexcept;
    bool operator!=(Point) const noexcept;
    auto friend operator<<(std::ostream&, Point const&) -> std::ostream&;
};
