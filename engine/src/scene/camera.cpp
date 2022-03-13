#include "scene/camera.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

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

auto Camera::react_key(unsigned char key, int x, int y) noexcept -> void {
    switch (_mode) {
    case CameraMode::ORBIT:
        react_key_orbit(key, x, y);
        break;
    case CameraMode::FPV:
        react_key_fpv(key, x, y);
        break;
    }
    switch(key) {
    case 'v':
        if (_mode == CameraMode::ORBIT) {
            glutSetCursor(GLUT_CURSOR_NONE);
            _mode = CameraMode::FPV;
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
            _mode = CameraMode::ORBIT;
        }
    }
}

auto Camera::react_key_orbit(unsigned char key, int x, int y) noexcept -> void {
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
    default:
        return;
    }

    if (beta < -1.5) {
        beta = -1.5;
    } else if (beta > 1.5) {
       beta = 1.5;
    }

    _eye = Point::spherical(radius, alpha, beta);
}

auto Camera::react_key_fpv(unsigned char key, int x, int y) noexcept -> void {
    auto vec = _center - _eye;
    vec.normalize();
    switch (key) {
    case 'w':
        vec = vec * 0.5;
        break;
    case 's':
        vec = vec * -0.5;
        break;
    default:
        return;
    }
    _eye = _eye + vec;
    _center = _center + vec;
}

auto Camera::cursor_motion(int center_x, int center_y, int x, int y) noexcept
    -> void
{
    if (_mode == CameraMode::FPV && (x != 0 || y != 0)) {
        auto vec = _center - _eye;

        auto radius = vec.radius();
        auto alpha = vec.alpha();
        auto beta = vec.beta();

        alpha += (center_x - x) * 0.001;
        beta += (center_y - y) * 0.001;

        if (beta < -1.5) {
            beta = -1.5;
        } else if (beta > 1.5) {
        beta = 1.5;
        }

        _center = _eye + Point::spherical(radius, alpha, beta);
    }
}

auto Camera::mode() const noexcept -> CameraMode {
    return _mode;
}

auto operator<<(std::ostream& oss, Camera const& c) -> std::ostream& {
    oss << "Camera:" << std::endl;
    oss << "  Eye: " << c._eye << std::endl;
    oss << "  Center: " << c._center << std::endl;
    oss << "  Up: " << c._up << std::endl;
    oss << "  Projection: " << c._projection << std::endl;
    oss << "  Camera Mode: ";
    if (c._mode == CameraMode::ORBIT) {
        oss << "Orbit" << std::endl;
    } else {
        oss << "FPV" << std::endl;
    }

    return oss;
}
