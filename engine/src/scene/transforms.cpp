#include "scene/transforms.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

void Translation::apply() const noexcept {
    glTranslatef(_coords.x(), _coords.y(), _coords.z());
}

void Rotation::apply() const noexcept {
    glRotatef(_angle, _coords.x(), _coords.y(), _coords.z());
}

void Scale::apply() const noexcept {
    glScalef(_coords.x(), _coords.y(), _coords.z());
}
