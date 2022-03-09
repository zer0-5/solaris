#include "shapes/sphere.hpp"
#include <math.h>

Sphere::Sphere(int argc, char **argv) {
    if (argc < 3){
        throw "wrong number of parameters";
    }

    radius = std::stof(argv[0]);
    n_slices = std::stof(argv[2]);
    n_stacks = std::stof(argv[3]);
}

std::vector<Point> Sphere::calculateCoords() const{
    std::vector<Point> coords;
    float stacks_height = M_PI / n_stacks;
    float alpha = (2 * M_PI) / n_slices;

    for(size_t stack = 0; stack < n_stacks; ++stack){
        float current_stack_beta = M_PI / 2 - stacks_height * stack;
        float next_stack_beta = M_PI / 2 - stacks_height * (stack + 1);

        for(size_t slice = 0; slice < n_slices; ++slice){
            float current_slice_alpha = alpha * slice;
            float next_slice_alpha = alpha * (slice + 1);
            // P3 --- P4
            // |      |
            // P1 --- P2
            Point p1 = Point(radius * cos(current_stack_beta) * sin(current_slice_alpha), radius * sin(current_stack_beta) , radius * cos(current_stack_beta) * current_slice_alpha);
            Point p2 = Point(radius * cos(current_stack_beta) * sin(next_slice_alpha), radius * sin(current_stack_beta) , radius * cos(current_stack_beta) * next_slice_alpha);
            Point p3 = Point(radius * cos(next_stack_beta) * sin(current_slice_alpha), radius * sin(next_stack_beta) , radius * cos(next_stack_beta) * current_slice_alpha);
            Point p4 = Point(radius * cos(next_stack_beta) * sin(next_slice_alpha), radius * sin(next_stack_beta) , radius * cos(next_stack_beta) * next_slice_alpha);

            // Na base p3 == p4 e no topo p1 == p2
            if(stack != 0){
                coords.push_back(p4);
                coords.push_back(p3);
                coords.push_back(p1);
            }
            if(stack != n_stacks - 1){
                coords.push_back(p4);
                coords.push_back(p1);
                coords.push_back(p2);
            }
        }
    }
    return coords;
}