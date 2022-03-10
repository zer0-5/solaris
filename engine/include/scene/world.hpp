#pragma once

#include "scene/camera.hpp"
#include "scene/group.hpp"

class World {
  private:
    Camera _camera;
    Group _group;

  public:
    World() : _camera(Camera()), _group(Group()) {}
    World(Camera&& camera, Group&& group)
      : _camera(std::move(camera))
      , _group(std::move(group)) {}
};
