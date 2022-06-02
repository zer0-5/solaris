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

std::vector<Vertex> Cylinder::calculateCoords() const {
    std::vector<Vertex> points;

    auto n_top = Vec3::cartesian(0.f, 1.f, 0.f);
    auto n_bot = Vec3::cartesian(0.f, -1.f, 0.f);

    auto t_top_center = Vec2(7.f / 16.f, 3.f / 16.f);
    auto t_bot_center = Vec2(13.f / 16.f, 3.f / 16.f);
    auto t_side = Vec2(0.f, 3.f / 4.f);
    auto t_side_height = 5.f / 8.f;
    auto t_radius = 3.f / 16.f;

    auto nsl = static_cast<float>(n_slices);
    auto nst = static_cast<float>(n_stacks);

    auto stack_height = height / nst;
    auto alpha = (2 * M_PI) / nsl;

    auto curr_height = 0.f;

    for (size_t stack = 0; stack < n_stacks; ++stack) {
        auto next_height = stack_height * (stack + 1);

        auto curr_alpha = 0;
        for (size_t slice = 0; slice < n_slices; ++slice) {
            auto next_alpha = alpha * (slice + 1);

            auto nc = Vec3::cartesian(sin(curr_alpha), 0, cos(curr_alpha));
            auto nn = Vec3::cartesian(sin(next_alpha), 0, cos(next_alpha));

            auto p1 = (radius * nc).sum_y(next_height);
            auto t1 = t_side + Vec2(slice, (stack + 1) * t_side_height) / nst;

            auto p2 = (radius * nn).sum_y(next_height);
            auto t2 = t_side + Vec2((slice + 1), (stack + 1) * t_side_height) / nst;

            auto p3 = (radius * nc).sum_y(curr_height);
            auto t3 = t_side + Vec2(slice, stack * t_side_height) / nst;

            auto p4 = (radius * nn).sum_y(curr_height);
            auto t4 = t_side + Vec2((slice + 1), stack * t_side_height) / nst;

            points.push_back(Vertex(p4, nn, t4));
            points.push_back(Vertex(p2, nn, t2));
            points.push_back(Vertex(p1, nc, t1));

            points.push_back(Vertex(p4, nn, t4));
            points.push_back(Vertex(p1, nc, t1));
            points.push_back(Vertex(p3, nc, t3));

            if (stack == 0) {
                // draw base
                Vec3 o = Vec3::cartesian(0, 0, 0);
                points.push_back(Vertex(p4, n_bot, t_bot_center + t_radius * Vec2(sin(next_alpha), cos(next_alpha))));
                points.push_back(Vertex(p3, n_bot, t_bot_center + t_radius * Vec2(sin(alpha), cos(alpha))));
                points.push_back(Vertex(o, n_bot, t_bot_center));
            }

            if (stack == n_stacks - 1){
                // draw top
                Vec3 o = Vec3::cartesian(0, next_height, 0);
                points.push_back(Vertex(o, n_top, t_top_center));
                points.push_back(Vertex(p1, n_top, t_top_center + t_radius * Vec2(sin(alpha), cos(alpha))));
                points.push_back(Vertex(p2, n_top, t_top_center + t_radius * Vec2(sin(next_alpha), cos(next_alpha))));
            }
            curr_alpha = next_alpha;
        }

        curr_height = next_height;

    }
    return points;
}
