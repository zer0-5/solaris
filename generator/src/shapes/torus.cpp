#include "shapes/torus.hpp"
#include <math.h>

Torus::Torus(int argc, char** argv){
    if (argc < 5) {
        throw "wrong number of parameters";
    }

    torus_radius = std::stof(argv[0]);
    out_radius = std::stof(argv[1]);
    n_slices = std::stof(argv[2]);
    n_stacks = std::stof(argv[3]);
}

std::vector<Point> Torus::calculateCoords() const {
    
    std::vector<Point> coords;

    double phi_step = 2 * M_PI / n_slices;
    double theta_step = 2 * M_PI / n_stacks;
    float curr_theta = 0;

    for(size_t stack = 1; stack <= n_stacks; stack++){
        float next_theta = theta_step * stack;

        float curr_phi = 0;
        for(size_t slice = 1; slice <= n_slices; slice++){
            float next_phi = phi_step * slice;
            
            Point p1 = Point::cartesian((out_radius + torus_radius * cos(curr_phi)) * cos(curr_theta), torus_radius * sin(curr_phi), (out_radius + torus_radius * cos(curr_phi)) * sin(curr_theta));
            Point p2 = Point::cartesian((out_radius + torus_radius * cos(next_phi)) * cos(curr_theta), torus_radius * sin(next_phi), (out_radius + torus_radius * cos(next_phi)) * sin(curr_theta));
            Point p3 = Point::cartesian((out_radius + torus_radius * cos(next_phi)) * cos(next_theta), torus_radius * sin(next_phi), (out_radius + torus_radius * cos(next_phi)) * sin(next_theta));
            Point p4 = Point::cartesian((out_radius + torus_radius * cos(curr_phi)) * cos(next_theta), torus_radius * sin(curr_phi), (out_radius + torus_radius * cos(curr_phi)) * sin(next_theta));

            coords.push_back(p1);
            coords.push_back(p2);
            coords.push_back(p4);

            coords.push_back(p2);
            coords.push_back(p3);
            coords.push_back(p4);

            curr_phi = next_phi;
        }

        curr_theta = next_theta;
    }
    return coords;
}
