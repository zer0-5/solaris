#include "scene/light.hpp"

cpp::result<GLenum, ParseError> Light::try_get_new_id() noexcept {
    static GLenum curr_light_id = GL_LIGHT0;
    if (curr_light_id > GL_LIGHT7) {
        return cpp::fail(ParseError::LIGHT_LIMIT);
    }
    return curr_light_id++;
}

void Light::enable() const noexcept {
    static GLfloat dark[4] = {0.2, 0.2, 0.2, 1.0};
    static GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};

    glEnable(_id);
    glLightfv(_id, GL_AMBIENT, dark);
    glLightfv(_id, GL_DIFFUSE, white);
    glLightfv(_id, GL_SPECULAR, white);
}

cpp::result<PointLight, ParseError> PointLight::try_new(Vec3 p) noexcept {
    auto id = Light::try_get_new_id();
    CHECK_RESULT(id);
    return PointLight(*id, p);
}

void PointLight::place() const noexcept {
    float pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};
    glLightfv(_id, GL_POSITION, pos);
}

cpp::result<DirectionalLight, ParseError> DirectionalLight::try_new(Vec3 d) noexcept {
    auto id = Light::try_get_new_id();
    CHECK_RESULT(id);
    return DirectionalLight(*id, d);
}

void DirectionalLight::place() const noexcept {
    float dir[4] = {_direction.x(), _direction.y(), _direction.z(), 0.0f};
    glLightfv(_id, GL_POSITION, dir);
}

cpp::result<SpotLight, ParseError> SpotLight::try_new(Vec3 p, Vec3 d, float cutoff) noexcept {
    auto id = Light::try_get_new_id();
    CHECK_RESULT(id);
    return SpotLight(*id, p, d, cutoff);
}

void SpotLight::place() const noexcept {
    float pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};
    float dir[4] = {_direction.x(), _direction.y(), _direction.z(), 0.0f};
    glLightfv(_id, GL_POSITION, pos);
    glLightfv(_id, GL_SPOT_DIRECTION, dir);
    glLightf(_id, GL_SPOT_CUTOFF, _cutoff);
}
