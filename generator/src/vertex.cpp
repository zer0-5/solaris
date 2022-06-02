#include "vertex.hpp"

auto operator<<(std::ostream& oss, Vertex const& p) -> std::ostream& {
    oss << p._position << ' ' << p._normal << ' ' << p._texcoord;
    return oss;
}
