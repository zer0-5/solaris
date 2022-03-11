#pragma once

#include "scene/model.hpp"
#include "tinyxml2.h"

#include <vector>

class Group {
  private:
    std::vector<Model> _models;
    std::vector<Group> _subgroups;

  public:
    Group() : _models(std::vector<Model>()), _subgroups(std::vector<Group>()){};
    Group(std::vector<Model> models, std::vector<Group> subgroups)
      : _models(std::move(models))
      , _subgroups(std::move(subgroups)) {}
    auto draw() const noexcept -> void;
};
