#pragma once

#include "scene/world.hpp"

class Renderer {
  public:
    Renderer();
    auto set_world(World&) noexcept -> Renderer&;
    auto run() const noexcept -> void;
};
