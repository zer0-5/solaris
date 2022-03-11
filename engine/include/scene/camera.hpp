#pragma once

#include "point.hpp"
#include "tinyxml2.h"

#include <iostream>

class Camera {
  private:
    Point _eye;
    Point _center;
    Point _up;
    Point _projection;

  public:
    Camera()
      : _eye(Point::cartesian(0, 0, 0))
      , _center(Point::cartesian(0, 0, 0))
      , _up(Point::cartesian(0, 0, 0))
      , _projection(Point::cartesian(0, 0, 0)) {}
    Camera(Point&& eye, Point&& center, Point&& up, Point&& projection)
      : _eye(std::move(eye))
      , _center(std::move(center))
      , _up(std::move(up))
      , _projection(std::move(projection)) {}
    auto place() const noexcept -> void;
    auto set_prespective(int, int) const noexcept -> void;
    auto react_key(unsigned char, int, int) noexcept -> void;
    auto friend operator<<(std::ostream&, Camera const&) -> std::ostream&;
};
