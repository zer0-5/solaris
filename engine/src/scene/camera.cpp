#include "scene/camera.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

#include <algorithm>

auto Camera::place() const noexcept -> void {
    if (_mode == CameraMode::FPV) {
        glutWarpPointer(_screen_width / 2.f, _screen_height / 2.f);
    }

    glLoadIdentity();
    gluLookAt(
        _eye.x(), _eye.y(), _eye.z(),
        _center.x(), _center.y(), _center.z(),
        _up.x(), _up.y(), _up.z()
    );
}

auto Camera::set_screen_size(int w, int h) noexcept -> void {
    _screen_width = w;
    _screen_height = h;
}

auto Camera::set_prespective() const noexcept -> void {
    gluPerspective(
        _projection.x(),
        _screen_width * 1.0f / _screen_height,
        _projection.y(),
        _projection.z()
    );
}

auto Camera::switch_mode() noexcept -> void {
    switch (_mode) {
    case CameraMode::ORBIT:
        glutSetCursor(GLUT_CURSOR_NONE);
        _mode = CameraMode::FPV;
        break;
    case CameraMode::FPV:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        _mode = CameraMode::ORBIT;
        break;
    default:
        break;
    }
}

auto Camera::react_key(
    std::array<bool, std::numeric_limits<unsigned char>::max()>& kb
) noexcept -> void {
    switch (_mode) {
    case CameraMode::ORBIT:
        react_key_orbit(kb);
        break;
    case CameraMode::FPV:
        react_key_fpv(kb);
        break;
    default:
        break;
    }
}

auto Camera::react_key_orbit(
    std::array<bool, std::numeric_limits<unsigned char>::max()>& kb
) noexcept -> void {
    auto beta = _eye.beta();
    auto radius = _eye.radius();
    auto alpha = _eye.alpha();

    if (kb['w']) beta += 0.1f;
    if (kb['s']) beta -= 0.1f;
    if (kb['a']) alpha -= 0.1f;
    if (kb['d']) alpha += 0.1f;
    if (kb['+']) radius -= 0.5f;
    if (kb['-']) radius += 0.5f;

    beta = std::clamp(beta, -1.5f, 1.5f);

    _eye = Vec3::spherical(radius, alpha, beta);
}

auto Camera::react_key_fpv(
    std::array<bool, std::numeric_limits<unsigned char>::max()>& kb
) noexcept -> void {
    auto vec = _center - _eye;
    vec.normalize();

    auto mov = Vec3::cartesian(0.f, 0.f, 0.f);

    if (kb['w']) mov += vec * 0.5f;
    if (kb['s']) mov -= vec * 0.5f;
    if (kb['a']) mov -= Vec3(vec).cross(_up).normalize() * 0.5f;
    if (kb['d']) mov += Vec3(vec).cross(_up).normalize() * 0.5f;
    if (kb['+']) mov += Vec3(_up) * 0.5f;
    if (kb['-']) mov -= Vec3(_up) * 0.5f;

    _eye += mov;
    _center += mov;
}

auto Camera::cursor_motion(int x, int y) noexcept -> void {
    if (_mode == CameraMode::FPV && (x != 0 || y != 0)) {
        auto vec = _center - _eye;

        auto radius = vec.radius();
        auto alpha = vec.alpha();
        auto beta = vec.beta();

        alpha += (_screen_width / 2. - x) * 0.001;
        beta += (_screen_height / 2. - y) * 0.001;

        if (beta < -1.5) {
            beta = -1.5;
        } else if (beta > 1.5) {
            beta = 1.5;
        }

        _center = _eye + Vec3::spherical(radius, alpha, beta);
    }
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
