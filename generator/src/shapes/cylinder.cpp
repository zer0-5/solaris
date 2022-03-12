#include "shapes/cylinder.hpp"
#include <math.h>

Cylinder::Cylinder(int argc, char** argv){
    if (argc < 5) {
        throw "wrong number of parameters";
    }

    radius = std::stof(argv[0]);
    height = std::stof(argv[1]);
    n_slices = std::stof(argv[2]);
    n_stacks = std::stof(argv[3]);
}

std::vector<Point> Cylinder::calculateCoords() const {
    std::vector<Point> coords;
    float stack_height = height / n_stacks;
    float alpha = (2 * M_PI) / n_slices;

    float curr_height = 0;

    for (size_t stack = 1; stack <= n_stacks; ++stack) {
        float next_height = stack_height * stack;

        float curr_alpha = 0;
        for (size_t slice = 1; slice <= n_slices; ++slice) {
            float next_alpha = alpha * slice;

            Point p1 = Point::cartesian(radius * sin(curr_alpha), next_height, radius * cos(curr_alpha));
            Point p2 = Point::cartesian(radius * sin(next_alpha), next_height, radius * cos(next_alpha));
            Point p3 = Point::cartesian(radius * sin(curr_alpha), curr_height, radius * cos(curr_alpha));
            Point p4 = Point::cartesian(radius * sin(next_alpha), curr_height, radius * cos(next_alpha));

            coords.push_back(p4);
            coords.push_back(p2);
            coords.push_back(p1);

            coords.push_back(p4);
            coords.push_back(p1);
            coords.push_back(p3);

            if (stack == 1) {
                // draw base
                Point o = Point::cartesian(0, 0, 0);
                coords.push_back(p4);
                coords.push_back(p3);
                coords.push_back(o);
            }
            
            if (stack == n_stacks){
                // draw top
                Point o = Point::cartesian(0, next_height, 0);
                coords.push_back(o);
                coords.push_back(p1);
                coords.push_back(p2);
                
            }

            curr_alpha = next_alpha;
            
        }

        curr_height = next_height;

    }
    return coords;
}
