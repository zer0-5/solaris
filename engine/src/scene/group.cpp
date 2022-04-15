#include "scene/group.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

auto Group::draw() const noexcept -> void {
    glPushMatrix();
    for (auto&& transform : _transforms) {
        transform->apply();
    }
    for (auto&& model : _models) {
        model.draw();
    }
    for (auto&& group : _subgroups) {
        group.draw();
    }
    glPopMatrix();
}
