#pragma once

#include "point.hpp"
#include "tinyxml2.h"

#include <iostream>
#include <array>
#define _USE_MATH_DEFINES
#include <math.h>

enum CameraMode {
    ORBIT,
    FPV
};

class Camera {
  private:
    Point _eye;
    Point _center;
    Point _up;
    Point _projection;
    CameraMode _mode;
    int _screen_width;
    int _screen_height;

    auto react_key_orbit(
      std::array<bool, std::numeric_limits<unsigned char>::max()>&
    ) noexcept -> void;
    auto react_key_fpv(
      std::array<bool, std::numeric_limits<unsigned char>::max()>&
    ) noexcept -> void;

  public:
    Camera()
      : _eye(Point::cartesian(0, 0, 0))
      , _center(Point::cartesian(0, 0, 0))
      , _up(Point::cartesian(0, 0, 0))
      , _projection(Point::cartesian(0, 0, 0))
      , _mode(CameraMode::ORBIT) {}
    Camera(Point&& eye, Point&& center, Point&& up, Point&& projection)
      : _eye(std::move(eye))
      , _center(std::move(center))
      , _up(std::move(up))
      , _projection(std::move(projection))
      , _mode(CameraMode::ORBIT) {}
    auto place() const noexcept -> void;
    auto set_screen_size(int, int) noexcept -> void;
    auto set_prespective() const noexcept -> void;
    auto switch_mode() noexcept -> void;
    auto react_key(
      std::array<bool, std::numeric_limits<unsigned char>::max()>&
    ) noexcept -> void;
    auto cursor_motion(int, int) noexcept -> void;
    auto friend operator<<(std::ostream&, Camera const&) -> std::ostream&;
};
