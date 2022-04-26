#include "render/renderer.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

#include <limits>

namespace state {
World static* world = nullptr;

auto static keyboard =
    std::array<bool, std::numeric_limits<unsigned char>::max()>();

auto static is_paused = false;
auto static debug_mode = false;
auto static polygon_mode = GL_FILL;
}

void change_size(int w, int h) {
    state::world->camera.set_screen_size(w, h);

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0) {
        h = 1;
    }

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

auto get_delta() -> int {
    static int prev_elapsed = 0;

    auto curr_elapsed = glutGet(GLUT_ELAPSED_TIME);
    auto delta =  curr_elapsed - prev_elapsed;
    prev_elapsed = curr_elapsed;

    return delta;
}

void render_scene(void) {
    static int elapsed_time = 0;
    auto delta = get_delta();
    if (!state::is_paused) {
        elapsed_time += delta;
    }

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    state::world->camera.place();
    state::world->camera.react_key(state::keyboard);

    // draw axis
    if (state::debug_mode) {
        draw_axis();
    }

    // draw groups
    glPolygonMode(GL_FRONT, state::polygon_mode);
    state::world->group.draw(elapsed_time / 1000.f, state::debug_mode);

    // end of frame
    glutSwapBuffers();
}

void handle_key_down(unsigned char key, int _x, int _y) {
    state::keyboard[key] = true;
}

void handle_key_up(unsigned char key, int _x, int _y) {
    state::keyboard[key] = false;
}

void handle_special_key(int key, int x, int y) {
    switch(key) {
    case GLUT_KEY_F1:
        state::world->camera.switch_mode();
        break;
    case GLUT_KEY_F2:
        state::debug_mode = !state::debug_mode;
        break;
    case GLUT_KEY_F3:
        state::polygon_mode = state::polygon_mode == GL_FILL ? GL_LINE : GL_FILL;
        break;
    case GLUT_KEY_F4:
        state::is_paused = !state::is_paused;
        break;
    default:
        break;
    }
}

void cursor_motion(int x, int y) {
    state::world->camera.cursor_motion(x, y);
}

Renderer::Renderer(World& world) {

    // init GLUT and the window
    static int glut_argc = 1;
    static char glut_arg[] = "";
    static char* glut_argv[] = {glut_arg};
    glutInit(&glut_argc, glut_argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("solaris");

    // Required callback registry
    glutDisplayFunc(render_scene);
    glutIdleFunc(render_scene);
    glutPassiveMotionFunc(cursor_motion);
    glutMotionFunc(cursor_motion);
    glutReshapeFunc(change_size);

    // Callback registration for keyboard processing
    glutKeyboardFunc(handle_key_down);
    glutKeyboardUpFunc(handle_key_up);
    glutSpecialFunc(handle_special_key);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    state::world = &world;
}

auto Renderer::run() const noexcept -> void {
    glutMainLoop();
}
