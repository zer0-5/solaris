#pragma once

#include "parse/parse_error.hpp"
#include "result.hpp"
#include "vec3.hpp"

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif
#include <memory>
#include <optional>
#include <vector>

class ModelBuffer {
  private:
    GLuint _positions, _normals, _texcoords;
    size_t _n_vertices;
    ModelBuffer(
        GLuint points,
        GLuint normals,
        GLuint texcoords,
        size_t n_vertices)
      : _positions(points)
      , _normals(normals)
      , _texcoords(texcoords)
      , _n_vertices(n_vertices) {}

  public:
    static auto try_from_file(std::string_view) noexcept
        -> cpp::result<ModelBuffer, ParseError>;
    auto draw() const noexcept -> void;
};

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;

    auto static from_rgb(int ir, int ig, int ib) noexcept -> Color {
        return Color{ir / 255.0f, ig / 255.0f, ib / 255.0f};
    }
};

class TextureBuffer {
  private:
    GLuint _id;
    TextureBuffer(GLuint id) : _id(id) {}

  public:
    static auto try_from_file(std::string_view) noexcept
        -> cpp::result<TextureBuffer, ParseError>;
    auto bind() const noexcept -> void;
};

class Model {
  private:
    ModelBuffer _model_buffer;
    std::optional<TextureBuffer> _texture_buffer;
    Color _diffuse;
    Color _ambient;
    Color _specular;
    Color _emissive;
    float _shininess;

  public:
    Model(
        ModelBuffer model_buffer,
        std::optional<TextureBuffer> texture_buffer,
        Color diffuse,
        Color ambient,
        Color specular,
        Color emissive,
        float shininess)
      : _model_buffer(model_buffer)
      , _texture_buffer(texture_buffer)
      , _diffuse(diffuse)
      , _ambient(ambient)
      , _specular(specular)
      , _emissive(emissive)
      , _shininess(shininess) {}
    auto draw() const noexcept -> void;
};
