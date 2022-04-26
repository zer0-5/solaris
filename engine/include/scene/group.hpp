#pragma once

#include "scene/model.hpp"
#include "scene/transforms.hpp"
#include "tinyxml2.h"

#include <memory>
#include <vector>

class Group {
  private:
    std::vector<Model> _models;
    std::vector<Group> _subgroups;
    std::vector<std::unique_ptr<Transform>> _transforms;

  public:
    Group() = default;
    Group(
        std::vector<Model> models,
        std::vector<Group> subgroups,
        std::vector<std::unique_ptr<Transform>> transforms)
      : _models(std::move(models))
      , _subgroups(std::move(subgroups))
      , _transforms(std::move(transforms)) {}
    auto draw(float, bool) const noexcept -> void;
};
