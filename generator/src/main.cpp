#include "shapes/plane.hpp"

#include <iostream>

static auto const help_string =
    "Usage: generator [PRIMITIVE] [ARGS] [FILE NAME]\n\n"
    "PRIMITIVE  ARGS\n"
    "plane      [side] [divisions]\n"
    "box        [x] [y] [z] [divisions]\n"
    "sphere     [radius] [slices] [stacks]\n"
    "cone       [radius] [height] [slices] [stacks]\n";

int main(int argc, char** argv) {
    if (argc < 2 || "--help" == std::string(argv[1])
        || "-h" == std::string(argv[1])) {
        std::cout << help_string << std::endl;
        return 0;
    }
    try {
        auto type = std::string_view(argv[1]);
        Primitive* p;
        if (type == "plane") {
            p = new Plane(argc - 2, argv + 2);
        } else {
            throw "invalid primitive";
        }

        for (auto& coord : p->calculateCoords()) {
            printf("%f %f %f\n", coord.x(), coord.y(), coord.z());
        }

        delete p;
    } catch (char const* err) {
        std::cout << "Error: " << err << std::endl;
        std::cout << help_string << std::endl;
    }
}
