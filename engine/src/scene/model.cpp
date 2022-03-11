#include "scene/model.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

// auto draw_triangle(Point a, Point b, Point c) noexcept -> void {
//     glVertex3f(a.x(), a.y(), a.z());
//     glVertex3f(b.x(), b.y(), b.z());
//     glVertex3f(c.x(), c.y(), c.z());
// }

auto Model::draw() const noexcept -> void {
    glBegin(GL_TRIANGLES);
    auto max = (float) _points.size();
    auto i = 0.f;
    for (auto&& p : _points) {
        glColor3f(i / max, (max - i) / max, 0.9);
        ++i;
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();
}

auto operator<<(std::ostream& oss, Model const& m) -> std::ostream& {
    for (auto const& point : m._points) {
        oss << '(' << point << ") ";
    }

    return oss;
}
