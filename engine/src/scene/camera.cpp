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
        _projection.z());
}

auto Camera::react_key(unsigned char key, int x, int y) noexcept -> void {
    auto radius = _eye.radius();
    auto alpha = _eye.alpha();
    auto beta = _eye.beta();
    switch (key) {
    case 'w':
        beta += 0.1;
        break;
    case 's':
        beta -= 0.1;
        break;
    case 'a':
        alpha -= 0.1;
        break;
    case 'd':
        alpha += 0.1;
        break;
    case '+':
        radius -= 0.5;
        break;
    case '-':
        radius += 0.5;
        break;
    }

    if (beta < -1.5) {
        beta = -1.5;
    } else if (beta > 1.5) {
       beta = 1.5;
    }

    _eye = Point::spherical(radius, alpha, beta);
}

auto operator<<(std::ostream& oss, Camera const& c) -> std::ostream& {
    oss << "Camera:" << std::endl;
    oss << "  Eye: " << c._eye << std::endl;
    oss << "  Center: " << c._center << std::endl;
    oss << "  Up: " << c._up << std::endl;
    oss << "  Projection: " << c._projection << std::endl;

    return oss;
}
