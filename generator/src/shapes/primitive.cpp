#include "shapes/primitive.hpp"

#include <fstream>

void Primitive::write(char const* const file_path) const {
    auto f = std::ofstream(file_path);
    auto pn = calculateCoords();
    for (auto&& p : calculateCoords()) {
        f << p << '\n';
    }
}
