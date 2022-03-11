#include "scene/camera.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>

auto Camera::place() const noexcept -> void {
    glLoadIdentity();
    gluLookAt(
        _eye.x(), _eye.y(), _eye.z(),
        _center.x(), _center.y(), _center.z(),
        _up.x(), _up.y(), _up.z()
    );
}

auto Camera::set_prespective(int w, int h) const noexcept -> void {
    gluPerspective(
        _projection.x(),
        w * 1.0 / h,
        _projection.y(),
        _projection.z()
    );
}

// FIXME
auto Camera::react_key(unsigned char key, int x, int y) noexcept -> void {
    switch (key) {
    case 'w':
        _eye = _eye + Point::spherical(1, M_PI / 16, M_PI / 2);
    case 'a':
        _eye = _eye + Point::spherical(1, M_PI / 2, -M_PI / 16);
    case 's':
        _eye = _eye + Point::spherical(1, -M_PI / 16, M_PI / 2);
    case 'd':
        _eye = _eye + Point::spherical(1, 0, M_PI / 16);
    }
}

auto operator<<(std::ostream& oss, Camera const& c) -> std::ostream& {
    oss << "Camera:" << std::endl;
    oss << "  Eye: " << c._eye << std::endl;
    oss << "  Center: " << c._center << std::endl;
    oss << "  Up: " << c._up << std::endl;
    oss << "  Projection: " << c._projection << std::endl;

    return oss;
}
