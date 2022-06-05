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

    auto nst = static_cast<float>(n_stacks);
    auto nsl = static_cast<float>(n_slices);

    auto phi_step = 2 * M_PI / nsl;
    auto theta_step = 2 * M_PI / nst;
    auto curr_theta = 0.f;

    for(size_t stack = 0; stack < n_stacks; stack++){
        auto next_theta = theta_step * (stack + 1);

        auto current_tex_y = stack / nst;
        auto next_tex_y = (stack + 1) / nst;

        auto curr_phi = 0.f;
        for(size_t slice = 0; slice < n_slices; slice++){
            auto next_phi = phi_step * (slice + 1);

            auto current_tex_x = slice / nsl;
            auto next_tex_x = (slice + 1) / nsl;

            Vec3 p1 = Vec3::cartesian(
                (out_radius + torus_radius * cos(curr_phi)) * cos(curr_theta),
                torus_radius * sin(curr_phi),
                (out_radius + torus_radius * cos(curr_phi)) * sin(curr_theta)
            );
            Vec3 p2 = Vec3::cartesian(
                (out_radius + torus_radius * cos(next_phi)) * cos(curr_theta),
                torus_radius * sin(next_phi),
                (out_radius + torus_radius * cos(next_phi)) * sin(curr_theta)
            );
            Vec3 p3 = Vec3::cartesian(
                (out_radius + torus_radius * cos(next_phi)) * cos(next_theta),
                torus_radius * sin(next_phi),
                (out_radius + torus_radius * cos(next_phi)) * sin(next_theta)
            );
            Vec3 p4 = Vec3::cartesian(
                (out_radius + torus_radius * cos(curr_phi)) * cos(next_theta),
                torus_radius * sin(curr_phi),
                (out_radius + torus_radius * cos(curr_phi)) * sin(next_theta)
            );

            Vec3 n1 = Vec3::cartesian(
                cos(curr_phi) * cos(curr_theta),
                sin(curr_phi),
                cos(curr_phi) * sin(curr_theta)
            );
            Vec3 n2 = Vec3::cartesian(
                cos(next_phi) * cos(curr_theta),
                sin(next_phi),
                cos(next_phi) * sin(curr_theta)
            );
            Vec3 n3 = Vec3::cartesian(
                cos(next_phi) * cos(next_theta),
                sin(next_phi),
                cos(next_phi) * sin(next_theta)
            );
            Vec3 n4 = Vec3::cartesian(
                cos(curr_phi) * cos(next_theta),
                sin(curr_phi),
                cos(curr_phi) * sin(next_theta)
            );

            Vec2 t1 = Vec2(current_tex_x, current_tex_y);
            Vec2 t2 = Vec2(next_tex_x, current_tex_y);
            Vec2 t3 = Vec2(next_tex_x, next_tex_y);
            Vec2 t4 = Vec2(current_tex_x, next_tex_y);

            points.push_back(Vertex(p1, n1, t1));
            points.push_back(Vertex(p2, n2, t2));
            points.push_back(Vertex(p4, n4, t4));

            points.push_back(Vertex(p2, n2, t2));
            points.push_back(Vertex(p3, n3, t3));
            points.push_back(Vertex(p4, n4, t4));

            curr_phi = next_phi;
        }

        curr_theta = next_theta;
    }
    return points;
}
