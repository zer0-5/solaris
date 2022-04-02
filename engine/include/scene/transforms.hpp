#pragma once

#include "point.hpp"

class Transform {
  public:
    virtual void apply() const noexcept = 0;
    virtual ~Transform() {}
};

class Translation: public Transform {
  private:
    Point _coords;

  public:
    Translation() : _coords(Point::cartesian(0, 0, 0)) {}
    Translation(Point coords) : _coords(coords) {}
    void apply() const noexcept;
};

class Rotation: public Transform {
  private:
    float _angle;
    Point _coords;

  public:
    Rotation() : _angle(0), _coords(Point::cartesian(0, 0, 0)) {}
    Rotation(float angle, Point coords) : _angle(angle), _coords(coords) {}
    void apply() const noexcept;
};

class Scale: public Transform {
  private:
    Point _coords;

  public:
    Scale() : _coords(Point::cartesian(1, 1, 1)) {}
    Scale(Point coords) : _coords(coords) {}
    void apply() const noexcept;
};
