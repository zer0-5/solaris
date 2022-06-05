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
    static cpp::result<GLenum, ParseError> try_get_new_id() noexcept;
    Light(GLenum id) : _id(id) {}

  public:
    void turn_on() const noexcept;
    virtual void place() const noexcept = 0;
};

class PointLight: public Light {
  private:
    Vec3 _position;
    PointLight(GLenum id, Vec3 position) : Light(id), _position(position) {}

  public:
    static cpp::result<PointLight, ParseError> try_new(Vec3) noexcept;
    void place() const noexcept override;
};

class DirectionalLight: public Light {
  private:
    Vec3 _direction;
    DirectionalLight(GLenum id, Vec3 direction)
      : Light(id)
      , _direction(direction) {}

  public:
    static cpp::result<DirectionalLight, ParseError> try_new(Vec3) noexcept;
    void place() const noexcept override;
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
    static cpp::result<SpotLight, ParseError> try_new(Vec3, Vec3, float) noexcept;
    void place() const noexcept override;
};
