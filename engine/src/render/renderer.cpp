#include "render/renderer.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl3.h"

#include <limits>

namespace state {
World static* world = nullptr;

auto static keyboard =
    std::array<bool, std::numeric_limits<unsigned char>::max()>();

auto static simulation_speed = 1.0f;
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

    ImGui_ImplGLUT_ReshapeFunc(w, h);
}

void draw_windows() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("General Controls");

    ImGui::Text("FPS: %f", io.Framerate);

    ImGui::Text("Simulation Speed:");
    ImGui::SameLine();
    ImGui::SliderFloat("##", &state::simulation_speed, -4.0f, 4.0f);

    ImGui::Text("Polygon Mode:");
    ImGui::SameLine();
    auto label = state::polygon_mode == GL_FILL ? "Fill" : "Line";
    if (ImGui::Button(label)) {
        state::polygon_mode = state::polygon_mode == GL_FILL ? GL_LINE : GL_FILL;
    };

    ImGui::Text("Show Lines:");
    ImGui::SameLine();
    label = state::debug_mode ? "True" : "False";
    if (ImGui::Button(label)) {
        state::debug_mode = !state::debug_mode;
    };

    ImGui::End();
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
    elapsed_time += delta * state::simulation_speed;

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
    glPolygonMode(GL_FRONT_AND_BACK, state::polygon_mode);
    state::world->group.draw(elapsed_time / 1000.f, state::debug_mode);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    draw_windows();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // end of frame
    glutSwapBuffers();
}

void handle_key_down(unsigned char key, int x, int y) {
    state::keyboard[key] = true;
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
}

void handle_key_up(unsigned char key, int x, int y) {
    state::keyboard[key] = false;
    ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);
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
    case GLUT_KEY_F10:
        state::simulation_speed--;
        break;
    case GLUT_KEY_F11:
        state::simulation_speed = !state::simulation_speed;
        break;
    case GLUT_KEY_F12:
        state::simulation_speed++;
        break;
    default:
        break;
    }
    ImGui_ImplGLUT_SpecialFunc(key, x, y);
}

void cursor_motion(int x, int y) {
    state::world->camera.cursor_motion(x, y);
    ImGui_ImplGLUT_MotionFunc(x, y);
}

Renderer::Renderer() {

    // init GLUT and the window
    static int glut_argc = 1;
    static char glut_arg[] = "";
    static char* glut_argv[] = {glut_arg};
    glutInit(&glut_argc, glut_argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("solaris");

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);

    // Required callback registry
    glutReshapeFunc(change_size);
    glutDisplayFunc(render_scene);
    glutIdleFunc(render_scene);
    glutPassiveMotionFunc(cursor_motion);
    glutMotionFunc(cursor_motion);
    glutKeyboardFunc(handle_key_down);
    glutKeyboardUpFunc(handle_key_up);
    glutSpecialFunc(handle_special_key);
    glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
    glutMouseFunc(ImGui_ImplGLUT_MouseFunc);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Setup Platform/Renderer backends
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL3_Init();
}

auto Renderer::set_world(World& world) noexcept -> Renderer& {
    state::world = &world;

    return *this;
}

auto Renderer::run() const noexcept -> void {
    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}
