#include "scene/group.hpp"

#include "parse_error.hpp"
#include "tinyxml2.h"

auto Group::draw() const noexcept -> void {
    for (auto&& model : _models) {
        model.draw();
    }
    for (auto&& group : _subgroups) {
        group.draw();
    }
}
