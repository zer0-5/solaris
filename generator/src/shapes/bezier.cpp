#include "shapes/bezier.hpp"

#include "curve.hpp"
#include "matrix.hpp"

#include <fstream>

Bezier::Bezier(int argc, char** argv) {
    if (argc < 3) {
        throw "wrong number of parameters";
    }

    patch_file = argv[0];
    tess = std::stoi(argv[1]);
}

auto get_patch_point(
    std::array<std::array<Point, 4>, 4>& patch, float u, float v
) noexcept -> Point {
    std::array<std::array<Point, 4>, 1> temp{};
    matrices::mult<float, Point, Point, 1, 4, 4>(
        {{{u*u*u, u*u, u, 1}}},
        patch,
        temp
    );

    std::array<std::array<Point, 1>, 1> point{};
    matrices::mult<Point, float, Point, 1, 4, 1>(
        temp,
        {{{v*v*v}, {v*v}, {v}, {1}}},
        point
    );
    return point[0][0];
}

auto Bezier::calculateCoords() const -> std::vector<Point> {
    std::ifstream file_stream(patch_file.data());
    if (!file_stream.is_open()) {
        throw "patch file not found";
    }

    size_t num_patches;
    file_stream >> num_patches;
    auto patches = std::vector<std::array<size_t, 16>>(num_patches);
    for (size_t i = 0; i < num_patches; ++i) {
        for (size_t j = 0; j < 16; ++j) {
            size_t idx;
            file_stream >> idx;
            file_stream.ignore();
            patches[i][j] = idx;
        }
    }

    size_t num_points;
    file_stream >> num_points;
    auto points = std::vector<Point>(num_points);
    for (size_t i = 0; i < num_points; ++i) {
        float x, y, z;
        file_stream >> x;
        file_stream.ignore();
        file_stream >> y;
        file_stream.ignore();
        file_stream >> z;
        file_stream.ignore();

        points[i] = Point::cartesian(x, y, z);
    }

    auto coords = std::vector<Point>();
    for (size_t i = 0; i < num_patches; ++i) {
        auto& curr_patch = patches[i];
        auto patch_mat = Curve::patch_matrix({{
            {points[curr_patch[0]],  points[curr_patch[1]],  points[curr_patch[2]],  points[curr_patch[3]]},
            {points[curr_patch[4]],  points[curr_patch[5]],  points[curr_patch[6]],  points[curr_patch[7]]},
            {points[curr_patch[8]],  points[curr_patch[9]],  points[curr_patch[10]], points[curr_patch[11]]},
            {points[curr_patch[12]], points[curr_patch[13]], points[curr_patch[14]], points[curr_patch[15]]},
        }});
        for (size_t ui = 0; ui < tess; ++ui) {
            auto u = ui / static_cast<float>(tess);
            auto nu = (ui + 1) / static_cast<float>(tess);

            for (size_t vi = 0; vi < tess; ++vi) {
                auto v = vi / static_cast<float>(tess);
                auto nv = (vi + 1) / static_cast<float>(tess);

                Point p0 = get_patch_point(patch_mat, u, v);
                Point p1 = get_patch_point(patch_mat, u, nv);
                Point p2 = get_patch_point(patch_mat, nu, v);
                Point p3 = get_patch_point(patch_mat, nu, nv);

                coords.push_back(p3);
                coords.push_back(p2);
                coords.push_back(p1);

                coords.push_back(p2);
                coords.push_back(p0);
                coords.push_back(p1);
            }
        }
    }

    return coords;
}
