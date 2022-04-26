#pragma once

#include "curves.hpp"
#include "point.hpp"

class Transform {
  public:
    virtual void apply(float) const noexcept = 0;
    virtual void debug_info() const noexcept {}
    virtual ~Transform() {}

};

class StaticTranslation: public Transform {
  private:
    Point _coords;

  public:
    StaticTranslation() : _coords(Point::cartesian(0, 0, 0)) {}
    StaticTranslation(Point coords) : _coords(coords) {}
    void apply(float) const noexcept override;
};

class TimedTranslation: public Transform {
  private:
    Curve _curve;
    std::vector<Point> _trajectory;
    float _time;
    bool _is_aligned;
    Point _prev_y;

    std::array<float, 16>
        build_rotation_matrix(Point, Point, Point) const noexcept;

  public:
    TimedTranslation(std::vector<Point>, float, bool);
    void apply(float) const noexcept override;
    void debug_info() const noexcept override;
};

class StaticRotation: public Transform {
  private:
    float _angle;
    Point _coords;

  public:
    StaticRotation() : _angle(0), _coords(Point::cartesian(0, 0, 0)) {}
    StaticRotation(float angle, Point coords) : _angle(angle), _coords(coords) {}
    void apply(float) const noexcept override;
};

class TimedRotation: public Transform {
  private:
    float _time;
    Point _coords;

  public:
    TimedRotation() : _time(0), _coords(Point::cartesian(0, 0, 0)) {}
    TimedRotation(float time, Point coords) : _time(time), _coords(coords) {}
    void apply(float) const noexcept override;
};

class Scale: public Transform {
  private:
    Point _coords;

  public:
    Scale() : _coords(Point::cartesian(1, 1, 1)) {}
    Scale(Point coords) : _coords(coords) {}
    void apply(float) const noexcept override;
};
