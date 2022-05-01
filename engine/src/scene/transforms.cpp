#include "scene/transforms.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

#define TESS 100

auto StaticTranslation::apply(float _elapsed_time) const noexcept -> void {
    glTranslatef(_coords.x(), _coords.y(), _coords.z());
}

TimedTranslation::TimedTranslation(std::vector<Point> points, float time, bool is_aligned)
    : _curve(Curve::catmull_rom(std::move(points)))
    , _trajectory(std::vector<Point>())
    , _time(time)
    , _is_aligned(is_aligned)
    , _prev_y(Point::cartesian(0, 1, 0))
{

    for (auto t = 0.0f; t < 1.0f; t += 1.0f / TESS) {
        _trajectory.push_back(_curve.get_position(t).first);
    }
}

auto TimedTranslation::apply(float elapsed_time) const noexcept -> void {
    auto [pos, dir] =  _curve.get_position(elapsed_time / _time);

    glTranslatef(pos.x(), pos.y(), pos.z());

    if (_is_aligned) {
        auto x = dir.normalize();
        auto z = Point(x).cross(_prev_y).normalize();
        auto y = Point(z).cross(x).normalize();
        glMultMatrixf(build_rotation_matrix(x, y, z).data());
    }
}

auto TimedTranslation::build_rotation_matrix(
    Point x, Point y, Point z
) const noexcept -> std::array<float, 16> {
    return std::array<float, 16>{{
        x.x(), x.y(), x.z(), 0,
        y.x(), y.y(), y.z(), 0,
        z.x(), z.y(), z.z(), 0,
            0,     0,     0, 1,
    }};
}

auto TimedTranslation::debug_info() const noexcept -> void {
    glBegin(GL_LINE_LOOP);
    for (auto&& point : _trajectory) {
        glVertex3f(point.x(), point.y(), point.z());
    }
    glEnd();
}

auto StaticRotation::apply(float _elapsed_time) const noexcept -> void {
    glRotatef(_angle, _coords.x(), _coords.y(), _coords.z());
}

auto TimedRotation::apply(float elapsed_time) const noexcept -> void {
    glRotatef(
        elapsed_time * (_time ? 360.0f / _time : 0),
        _coords.x(), _coords.y(), _coords.z()
    );
}

auto Scale::apply(float _elapsed_time) const noexcept -> void {
    glScalef(_coords.x(), _coords.y(), _coords.z());
}
