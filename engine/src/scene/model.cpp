#include "scene/model.hpp"

auto operator<<(std::ostream& oss, Model const& m) -> std::ostream& {
    for (auto const& point : m._points) {
        oss << '(' << point << ") ";
    }

    return oss;
}
