#pragma once

#include "vec2.hpp"
#include "vec3.hpp"

class Vertex {
  private:
    Vec3 _position;
    Vec3 _normal;
    Vec2 _texcoord;

  public:
    Vertex(Vec3 p, Vec3 n, Vec2 tc)
      : _position(p)
      , _normal(n)
      , _texcoord(tc) {}
    auto friend operator<<(std::ostream&, Vertex const&) -> std::ostream&;
};
