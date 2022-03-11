#include "shapes/primitive.hpp"

#include <fstream>

void Primitive::write(char const* const file_path) const {
    auto f = std::ofstream(file_path);
    for (auto const& p : calculateCoords()) {
        f << p << std::endl;
    }
}
