#pragma once

#include "curve.hpp"
#include "vec3.hpp"

class Transform {
  public:
    virtual void apply(float) const noexcept = 0;
    virtual void debug_info() const noexcept {}
    virtual ~Transform() {}

};

class StaticTranslation: public Transform {
  private:
    Vec3 _coords;

  public:
    StaticTranslation() : _coords(Vec3::cartesian(0, 0, 0)) {}
    StaticTranslation(Vec3 coords) : _coords(coords) {}
    void apply(float) const noexcept override;
};

class TimedTranslation: public Transform {
  private:
    Curve _curve;
    std::vector<Vec3> _trajectory;
    float _time;
    bool _is_aligned;
    Vec3 _prev_y;

    std::array<float, 16>
        build_rotation_matrix(Vec3, Vec3, Vec3) const noexcept;

  public:
    TimedTranslation(std::vector<Vec3>, float, bool);
    void apply(float) const noexcept override;
    void debug_info() const noexcept override;
};

class StaticRotation: public Transform {
  private:
    float _angle;
    Vec3 _coords;

  public:
    StaticRotation() : _angle(0), _coords(Vec3::cartesian(0, 0, 0)) {}
    StaticRotation(float angle, Vec3 coords) : _angle(angle), _coords(coords) {}
    void apply(float) const noexcept override;
};

class TimedRotation: public Transform {
  private:
    float _time;
    Vec3 _coords;

  public:
    TimedRotation() : _time(0), _coords(Vec3::cartesian(0, 0, 0)) {}
    TimedRotation(float time, Vec3 coords) : _time(time), _coords(coords) {}
    void apply(float) const noexcept override;
};

class Scale: public Transform {
  private:
    Vec3 _coords;

  public:
    Scale() : _coords(Vec3::cartesian(1, 1, 1)) {}
    Scale(Vec3 coords) : _coords(coords) {}
    void apply(float) const noexcept override;
};
