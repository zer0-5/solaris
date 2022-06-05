#pragma once

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

#include "parse/parse_error.hpp"
#include "result.hpp"
#include "vec3.hpp"

class Light {
  protected:
    GLenum _id;
    auto static try_get_new_id() noexcept -> cpp::result<GLenum, ParseError>;
    Light(GLenum id) : _id(id) {}

  public:
    void turn_on() const noexcept;
    auto virtual place() const noexcept -> void = 0;
};

class PointLight: public Light {
  private:
    Vec3 _position;
    PointLight(GLenum id, Vec3 position) : Light(id), _position(position) {}

  public:
    auto static try_new(Vec3) noexcept -> cpp::result<PointLight, ParseError>;
    auto place() const noexcept -> void override;
};

class DirectionalLight: public Light {
  private:
    Vec3 _direction;
    DirectionalLight(GLenum id, Vec3 direction)
      : Light(id)
      , _direction(direction) {}

  public:
    auto static try_new(Vec3) noexcept
      -> cpp::result<DirectionalLight, ParseError>;
    auto place() const noexcept -> void override;
};

class SpotLight: public Light {
  private:
    Vec3 _position;
    Vec3 _direction;
    float _cutoff;
    SpotLight(GLenum id, Vec3 position, Vec3 direction, float cutoff)
      : Light(id)
      , _position(position)
      , _direction(direction)
      , _cutoff(cutoff) {}

  public:
    auto static try_new(Vec3, Vec3, float) noexcept
      -> cpp::result<SpotLight, ParseError>;
    auto place() const noexcept -> void override;
};
