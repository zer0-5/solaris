#include "scene/model.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

auto Model::draw() const noexcept -> void {
    glBegin(GL_TRIANGLES);
    glColor3f(_color.r / 255, _color.g / 255, _color.b / 255);
    for (auto&& p : (*_points)) {
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();
}

auto operator<<(std::ostream& oss, Model const& m) -> std::ostream& {
    for (auto const& point : (*m._points)) {
        oss << '(' << point << ") ";
    }

    return oss;
}
