#include "logger.hpp"

#include "fmt/color.h"

void error(char const* const msg) {
    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "error: ");
    fmt::print("{}\n", msg);
}

void warn(char const* const msg) {
    fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold, "warning: ");
    fmt::print("{}\n", msg);
}
