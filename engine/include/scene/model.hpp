#pragma once

#include "point.hpp"
#include "result.hpp"
#include "parse/parse_error.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif
#include <memory>
#include <vector>

class ModelBuffer {
  private:
    GLuint _vbo;
    size_t _n_vertices;

  public:
    ModelBuffer(GLuint vbo, size_t n_vertices)
      : _vbo(vbo)
      , _n_vertices(n_vertices) {}
    static auto try_from_file(std::string_view) noexcept
        -> cpp::result<ModelBuffer, ParseError>;
    auto draw() const noexcept -> void;
};

struct Color {
    float r;
    float g;
    float b;
};

class Model {
  private:
    ModelBuffer _buffer;
    Color _color;

  public:
    Model(ModelBuffer buffer, Color color)
      : _buffer(buffer)
      , _color(color) {}
    auto draw() const noexcept -> void;
};
