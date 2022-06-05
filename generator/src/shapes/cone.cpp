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

std::vector<Vertex> Cone::calculateCoords() const {
    std::vector<Vertex> points;

    auto nst = static_cast<float>(n_stacks);
    auto nsl = static_cast<float>(n_slices);

    auto stack_height = height / nst;
    auto alpha = (2 * M_PI) / nsl;
    auto beta = atan(radius / height);
    auto radius_step = -radius / nst;

    auto curr_radius = radius;
    auto curr_height = 0.0f;

    auto n_bot = Vec3::cartesian(0, -1, 0);

    auto t_top_center = Vec2(1.f / 4.f, 1.f / 2.f);
    auto t_bot_center = Vec2(3.f / 4.f, 1.f / 2.f);
    auto t_radius = Vec2(1.f / 4.f, 1.f / 2.f);

    for (size_t stack = 0; stack < n_stacks; ++stack) {
        auto next_radius = radius + (radius_step * (stack + 1));
        auto next_height = stack_height * (stack + 1);

        auto curr_alpha = 0.f;
        for (size_t slice = 0; slice < n_slices; ++slice) {

            auto next_alpha = alpha * (slice + 1);

            auto n_side = Vec3::spherical(1.f, curr_alpha, beta);
            auto n_side_next = Vec3::spherical(1.f, next_alpha, beta);

            auto p1 = Vec3::cartesian(next_radius * sin(curr_alpha), next_height, next_radius * cos(curr_alpha));
            auto p2 = Vec3::cartesian(next_radius * sin(next_alpha), next_height, next_radius * cos(next_alpha));
            auto p3 = Vec3::cartesian(curr_radius * sin(curr_alpha), curr_height, curr_radius * cos(curr_alpha));
            auto p4 = Vec3::cartesian(curr_radius * sin(next_alpha), curr_height, curr_radius * cos(next_alpha));

            auto t1 = t_top_center + next_radius * t_radius * Vec2(sin(curr_alpha), cos(curr_alpha));
            auto t2 = t_top_center + next_radius * t_radius * Vec2(sin(next_alpha), cos(next_alpha));
            auto t3 = t_top_center + curr_radius * t_radius * Vec2(sin(curr_alpha), cos(curr_alpha));
            auto t4 = t_top_center + curr_radius * t_radius * Vec2(sin(next_alpha), cos(next_alpha));

            auto vtx1 = Vertex(p1, n_side, t1);
            auto vtx2 = Vertex(p2, n_side_next, t2);
            auto vtx3 = Vertex(p3, n_side, t3);
            auto vtx4 = Vertex(p4, n_side_next, t4);

            if (stack != n_stacks - 1) {
                points.push_back(vtx4);
                points.push_back(vtx2);
                points.push_back(vtx1);
            }

            points.push_back(vtx4);
            points.push_back(vtx1);
            points.push_back(vtx3);

            if (stack == 0) {
                // draw base
                auto o = Vec3::cartesian(0, 0, 0);
                points.push_back(Vertex(p4, n_bot, t_bot_center + t_radius * Vec2(sin(next_alpha), cos(next_alpha))));
                points.push_back(Vertex(p3, n_bot, t_bot_center + t_radius * Vec2(sin(curr_alpha), cos(curr_alpha))));
                points.push_back(Vertex(o, n_bot, t_bot_center));
            }

            curr_alpha = next_alpha;
        }

        curr_radius = next_radius;
        curr_height = next_height;
    }
    return points;
}
