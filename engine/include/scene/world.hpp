#pragma once

#include "scene/camera.hpp"
#include "scene/group.hpp"

class World {
  public:
    Camera camera;
    Group group;
    World() = default;
    World(Camera&& camera, Group&& group)
      : camera(std::move(camera))
      , group(std::move(group)) {}
};
