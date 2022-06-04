#pragma once

#include "scene/camera.hpp"
#include "scene/group.hpp"
#include "scene/light.hpp"

class World {
  public:
    Camera camera;
    Group group;
    std::vector<std::unique_ptr<Light>> lights;
    World() = default;
    World(
        Camera&& camera,
        Group&& group,
        std::vector<std::unique_ptr<Light>>&& lights)
      : camera(std::move(camera))
      , group(std::move(group))
      , lights(std::move(lights)) {}
};
