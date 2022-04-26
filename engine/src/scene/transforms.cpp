#include "scene/transforms.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

#define TESS 100

void StaticTranslation::apply(float _elapsed_time) const noexcept {
    glTranslatef(_coords.x(), _coords.y(), _coords.z());
}

Translation::Translation(std::vector<Point> points, float time, bool is_aligned)
    : _curve(Curve::catmull_ron(std::move(points)))
    , _trajectory(std::vector<Point>())
    , _time(time)
    , _is_aligned(is_aligned)
    , _prev_y(Point::cartesian(0, 1, 0))
{

    for (auto t = 0.0f; t < 1.0f; t += 1.0f / TESS) {
        _trajectory.push_back(_curve.get_position(t).first);
    }
}

void Translation::apply(float elapsed_time) const noexcept {
    auto [pos, dir] =  _curve.get_position(elapsed_time / _time);

    glTranslatef(pos.x(), pos.y(), pos.z());

    if (_is_aligned) {
        auto x = dir.normalize();
        auto z = Point(x).cross(_prev_y).normalize();
        auto y = Point(z).cross(x).normalize();
        glMultMatrixf(build_rotation_matrix(x, y, z).data());
    }
}

std::array<float, 16> Translation::build_rotation_matrix(
    Point x, Point y, Point z
) const noexcept {
    return std::array<float, 16>{{
        x.x(), x.y(), x.z(), 0,
        y.x(), y.y(), y.z(), 0,
        z.x(), z.y(), z.z(), 0,
            0,     0,     0, 1,
    }};
}

void Translation::debug_info() const noexcept {
    glBegin(GL_LINE_LOOP);
    for (auto&& point : _trajectory) {
        glVertex3f(point.x(), point.y(), point.z());
    }
    glEnd();
}

void Rotation::apply(float _elapsed_time) const noexcept {
    glRotatef(_angle, _coords.x(), _coords.y(), _coords.z());
}

void Scale::apply(float _elapsed_time) const noexcept {
    glScalef(_coords.x(), _coords.y(), _coords.z());
}
