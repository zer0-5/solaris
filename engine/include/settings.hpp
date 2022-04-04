#pragma once

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

namespace settings {
auto static enable_axis = false;
auto static polygon_mode = GL_FILL;
}
