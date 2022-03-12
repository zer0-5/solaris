#include "shapes/sphere.hpp"

#include <math.h>

Sphere::Sphere(int argc, char** argv) {
    if (argc < 4) {
        throw "wrong number of parameters";
    }

    radius = std::stof(argv[0]);
    n_slices = std::stof(argv[1]);
    n_stacks = std::stof(argv[2]);
}

std::vector<Point> Sphere::calculateCoords() const {
    std::vector<Point> coords;
    float stacks_height = M_PI / n_stacks;
    float alpha = (2 * M_PI) / n_slices;

    float current_stack_beta = M_PI / 2;
    for (size_t stack = 1; stack <= n_stacks; ++stack) {
        float next_stack_beta = M_PI / 2 - stacks_height * stack;

        float current_slice_alpha = 0;
        for (size_t slice = 1; slice <= n_slices; ++slice) {
            float next_slice_alpha = alpha * slice;
            // P3 --- P4
            // |      |
            // P1 --- P2
            Point p1 = Point::spherical(radius, current_slice_alpha, current_stack_beta);
            Point p2 = Point::spherical(radius, next_slice_alpha, current_stack_beta);
            Point p3 = Point::spherical(radius, current_slice_alpha, next_stack_beta);
            Point p4 = Point::spherical(radius, next_slice_alpha, next_stack_beta);

            if (stack != n_stacks) {
                coords.push_back(p1);
                coords.push_back(p3);
                coords.push_back(p4);
            }

            if (stack != 1) {
                coords.push_back(p2);
                coords.push_back(p1);
                coords.push_back(p4);
            }

            current_slice_alpha = next_slice_alpha;
        }

        current_stack_beta = next_stack_beta;
    }
    return coords;
}
