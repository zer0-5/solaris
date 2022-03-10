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
      : _eye(Point())
      , _center(Point())
      , _up(Point())
      , _projection(Point()) {}
    Camera(Point&& eye, Point&& center, Point&& up, Point&& projection)
      : _eye(std::move(eye))
      , _center(std::move(center))
      , _up(std::move(up))
      , _projection(std::move(projection)) {}
    auto friend operator<<(std::ostream&, Camera const&) -> std::ostream&;
};
