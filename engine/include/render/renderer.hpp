#pragma once

#include "scene/world.hpp"

class Renderer {
  public:
    Renderer(World& world);
    auto run() const noexcept -> void;
};
