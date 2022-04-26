#include "scene/group.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

auto Group::draw(float elapsed_time, bool debug) const noexcept -> void {
    glPushMatrix();
    for (auto&& transform : _transforms) {
        if (debug) {
            transform->debug_info();
        }
        transform->apply(elapsed_time);
    }
    for (auto&& model : _models) {
        model.draw();
    }
    for (auto&& group : _subgroups) {
        group.draw(elapsed_time, debug);
    }
    glPopMatrix();
}
