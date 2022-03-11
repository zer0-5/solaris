#include "shapes/cone.hpp"
#include <math.h>

Cone::Cone(int argc, char** argv){
    if (argc < 5) {
        throw "wrong number of parameters";
    }

    radius = std::stof(argv[0]);
    height = std::stof(argv[1]);
    n_slices = std::stof(argv[2]);
    n_stacks = std::stof(argv[3]);
}

std::vector<Point> Cone::calculateCoords() const {
    std::vector<Point> coords;
    float stack_height = height / n_stacks;
    float alpha = (2 * M_PI) / n_slices;
    float radius_step = -radius / n_stacks;

    for (size_t stack = 0; stack < n_stacks; ++stack) {
        float curr_radius = radius + (radius_step * stack);
        float next_radius = radius + (radius_step * (stack + 1));
        float curr_height = stack_height * stack;
        float next_height = stack_height * (stack + 1);

        for (size_t slice = 0; slice < n_slices; ++slice) {
            float curr_alpha = alpha * slice;
            float next_alpha = alpha * (slice + 1);

            Point p1 = Point::cartesian(next_radius * sin(curr_alpha), next_height, next_radius * cos(curr_alpha));
            Point p2 = Point::cartesian(next_radius * sin(next_alpha), next_height, next_radius * cos(next_alpha));
            Point p3 = Point::cartesian(curr_radius * sin(curr_alpha), curr_height, curr_radius * cos(curr_alpha));
            Point p4 = Point::cartesian(curr_radius * sin(next_alpha), curr_height, curr_radius * cos(next_alpha));

            if (stack != n_stacks - 1) {
                coords.push_back(p4);
                coords.push_back(p2);
                coords.push_back(p1);
            }

            coords.push_back(p4);
            coords.push_back(p1);
            coords.push_back(p3);

            if (stack == 0) {
                // draw base
                Point o = Point::cartesian(0, 0, 0);
                coords.push_back(p4);
                coords.push_back(p3);
                coords.push_back(o);
            }
        }
    }
    return coords;
}
