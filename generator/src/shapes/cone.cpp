#include "shapes/cone.hpp"
#include <math.h>

Cone::Cone(int argc, char** argv){
    if (argc < 4){
        throw "wrong number of parameters";
    }

    radius = std::stof(argv[0]);
    height = std::stof(argv[1]);
    n_slices = std::stof(argv[2]);
    n_stacks = std::stof(argv[3]);
}

std::vector<Point> Cone::calculateCoords() const{
    std::vector<Point> coords;
    float stacks_height = height / n_stacks;
    float alpha = (2 * M_PI) / n_slices;
    for(size_t stack = 0; stack < n_stacks; ++stack){
        float current_stack_height = stacks_height * stack;
        float next_stack_height = stacks_height * (stack + 1);
        float current_stack_radius = (radius * (height - current_stack_height)) / height;
        float next_stack_radius = (radius * (height - next_stack_height)) / height;
        for(size_t slice = 0; slice < n_slices; ++slice){
            float current_slice_alpha = alpha * slice;
            float next_slice_alpha = alpha * (slice + 1);
            // P3 --- P4
            // |      |
            // P1 --- P2
            Point p1 = Point(cos(current_slice_alpha) * current_stack_radius, current_stack_height, -sin(current_slice_alpha) * current_stack_radius);
            Point p2 = Point(cos(next_slice_alpha) * current_stack_radius, current_stack_height, -sin(next_slice_alpha) * current_stack_radius);
            Point p3 = Point(cos(current_slice_alpha) * next_stack_radius, next_stack_height, -sin(current_slice_alpha) * next_stack_radius);
            Point p4 = Point(cos(next_slice_alpha) * next_stack_radius, next_stack_height, -sin(next_slice_alpha) * next_stack_radius);

            coords.push_back(p2);
            coords.push_back(p4);
            coords.push_back(p3);

            if(stack != n_stacks){
                coords.push_back(p3);
                coords.push_back(p1);
                coords.push_back(p2);
            }
            else{
                // top
                coords.push_back(p2);
                coords.push_back(p4);
                coords.push_back(p3);
            }
        }
    }
    return coords;
}