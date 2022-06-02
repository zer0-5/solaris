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

std::vector<Vertex> Sphere::calculateCoords() const {
    auto points = std::vector<Vertex>();
    auto stacks_height = static_cast<float>(M_PI / n_stacks);
    auto alpha = static_cast<float>((2 * M_PI) / n_slices);
    auto nsl = static_cast<float>(n_slices);
    auto nst = static_cast<float>(n_stacks);

    auto current_stack_beta = M_PI / 2.f;
    for (size_t stack = 1; stack < n_stacks; ++stack) {
        auto next_stack_beta = M_PI / 2.f - stacks_height * stack;
        auto current_slice_alpha = 0.0f;

        auto current_tex_y = stack / nst;
        auto next_tex_y = (stack + 1) / nst;

        for (size_t slice = 1; slice < n_slices; ++slice) {
            auto next_slice_alpha = alpha * slice;
            auto current_tex_x = slice / nsl;
            auto next_tex_x = (slice + 1) / nsl;
            // P3 --- P4
            // |      |
            // P1 --- P2
            Vec3 p1 = Vec3::spherical(radius, current_slice_alpha, current_stack_beta);
            Vec3 p2 = Vec3::spherical(radius, next_slice_alpha, current_stack_beta);
            Vec3 p3 = Vec3::spherical(radius, current_slice_alpha, next_stack_beta);
            Vec3 p4 = Vec3::spherical(radius, next_slice_alpha, next_stack_beta);


            if (stack != n_stacks - 1) {
                points.push_back(Vertex(p1, p1.normalize(), Vec2(current_tex_x, current_tex_y)));
                points.push_back(Vertex(p3, p3.normalize(), Vec2(current_tex_x, next_tex_y)));
                points.push_back(Vertex(p4, p4.normalize(), Vec2(next_tex_x, next_tex_y)));
            }

            if (stack != 0) {
                points.push_back(Vertex(p2, p2.normalize(), Vec2(current_tex_x, next_tex_y)));
                points.push_back(Vertex(p1, p1.normalize(), Vec2(current_tex_x, current_tex_y)));
                points.push_back(Vertex(p4, p4.normalize(), Vec2(next_tex_x, next_tex_y)));
            }

            current_slice_alpha = next_slice_alpha;
        }

        current_stack_beta = next_stack_beta;
    }
    return points;
}