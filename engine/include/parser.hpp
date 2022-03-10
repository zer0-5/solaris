#pragma once

#include "parse_error.hpp"
#include "result.hpp"
#include "scene/world.hpp"

#include <string_view>

auto parse(std::string_view) noexcept -> cpp::result<World, ParseError>;
