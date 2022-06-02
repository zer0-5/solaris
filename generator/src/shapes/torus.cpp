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

std::vector<Vertex> Torus::calculateCoords() const {
    
    std::vector<Vertex> points;

    auto phi_step = static_cast<double>(2 * M_PI / n_slices);
    auto theta_step = static_cast<double>(2 * M_PI / n_stacks);
    auto curr_theta = 0.f;

    auto nst = static_cast<float>(n_stacks);
    auto nsl = static_cast<float>(n_slices);

    for(size_t stack = 1; stack < n_stacks; stack++){
        auto next_theta = static_cast<float>(theta_step * (stack + 1));
        
        auto current_tex_x = stack / nst;
        auto next_tex_x = (stack + 1) / nst;

        auto curr_phi = 0.f;
        for(size_t slice = 1; slice < n_slices; slice++){
            auto next_phi = static_cast<float>(phi_step * (slice + 1));
            
            auto current_tex_y = slice / nsl;
            auto next_tex_y = (slice + 1) / nsl;

            Vec3 p1 = Vec3::cartesian((out_radius + torus_radius * cos(curr_phi)) * cos(curr_theta), torus_radius * sin(curr_phi), (out_radius + torus_radius * cos(curr_phi)) * sin(curr_theta));
            Vec3 p2 = Vec3::cartesian((out_radius + torus_radius * cos(next_phi)) * cos(curr_theta), torus_radius * sin(next_phi), (out_radius + torus_radius * cos(next_phi)) * sin(curr_theta));
            Vec3 p3 = Vec3::cartesian((out_radius + torus_radius * cos(next_phi)) * cos(next_theta), torus_radius * sin(next_phi), (out_radius + torus_radius * cos(next_phi)) * sin(next_theta));
            Vec3 p4 = Vec3::cartesian((out_radius + torus_radius * cos(curr_phi)) * cos(next_theta), torus_radius * sin(curr_phi), (out_radius + torus_radius * cos(curr_phi)) * sin(next_theta));

            Vec3 n1 = Vec3::cartesian(torus_radius * cos(curr_phi) * cos(curr_theta), torus_radius * sin(curr_phi), torus_radius * cos(curr_phi) * sin(curr_theta)).normalize();
            Vec3 n2 = Vec3::cartesian(torus_radius * cos(next_phi) * cos(curr_theta), torus_radius * sin(next_phi), torus_radius * cos(next_phi) * sin(curr_theta)).normalize();
            Vec3 n3 = Vec3::cartesian(torus_radius * cos(next_phi) * cos(next_theta), torus_radius * sin(next_phi), torus_radius * cos(next_phi) * sin(next_theta)).normalize();
            Vec3 n4 = Vec3::cartesian(torus_radius * cos(curr_phi) * cos(next_theta), torus_radius * sin(curr_phi), torus_radius * cos(curr_phi) * sin(next_theta)).normalize();


            points.push_back(Vertex(p1, n1, Vec2(current_tex_x, current_tex_y)));
            points.push_back(Vertex(p2, n2, Vec2(next_tex_x, current_tex_y)));
            points.push_back(Vertex(p4, n4, Vec2(next_tex_x, next_tex_y)));

            points.push_back(Vertex(p2, n2, Vec2(next_tex_x, current_tex_y)));
            points.push_back(Vertex(p3, n3, Vec2(current_tex_x, next_tex_y)));
            points.push_back(Vertex(p4, n4, Vec2(next_tex_x, next_tex_y)));

            curr_phi = next_phi;
        }

        curr_theta = next_theta;
    }
    return points;
}
