#include "render/renderer.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

namespace state {
World static* world = nullptr;

auto static enable_axis = false;
auto static polygon_mode = GL_FILL;
}

void change_size(int w, int h) {
    state::world->camera.set_screen_size(w, h);

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    state::world->camera.set_prespective();

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void draw_axis() {
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
}

void render_scene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    state::world->camera.place();

    // draw axis
    if (state::enable_axis) {
        draw_axis();
    }

    // draw groups
    glPolygonMode(GL_FRONT, state::polygon_mode);
    state::world->group.draw();

    // end of frame
    glutSwapBuffers();
}

void process_keys(unsigned char key, int x, int y) {
    state::world->camera.react_key(key, x, y);
    glutPostRedisplay();
}

void cursor_motion(int x, int y) {
    state::world->camera.cursor_motion(x, y);
    glutPostRedisplay();
}

Renderer::Renderer(World& world) {

    // init GLUT and the window
    auto glut_argc = 0;
    glutInit(&glut_argc, nullptr);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("solaris");

    // Required callback registry
    glutDisplayFunc(render_scene);
    // glutIdleFunc(render_scene);
    glutPassiveMotionFunc(cursor_motion);
    glutMotionFunc(cursor_motion);
    glutReshapeFunc(change_size);

    // Callback registration for keyboard processing
    glutKeyboardFunc(process_keys);
    // glutSpecialFunc(process_special_keys);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    state::world = &world;
}

auto Renderer::run() const noexcept -> void {
    glutMainLoop();
}
