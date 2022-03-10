#include "logger.hpp"
#include "parse_error.hpp"
#include "parser.hpp"
#include "scene/world.hpp"

// singletons
World WORLD;

int main(int argc, char** argv) {
    auto world_res = parse(argv[1]);
    if (world_res.has_error()) {
        error(error_msg(world_res.error()));
        return 1;
    } else {
        WORLD = world_res.value();
    }

    return 0;
}
