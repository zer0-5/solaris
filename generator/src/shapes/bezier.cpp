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
    std::array<std::array<Vec3, 4>, 4>& patch, float u, float v
) noexcept -> std::pair<Vec3, Vec3> {

    std::array<std::array<float, 4>, 1> mu = {{{u*u*u, u*u, u, 1}}};
    std::array<std::array<float, 4>, 1> du = {{{3*u*u, 2*u, 1, 0}}};

    std::array<std::array<float, 1>, 4> mv = {{{v*v*v}, {v*v}, {v}, {1}}};
    std::array<std::array<float, 1>, 4> dv = {{{3*v*v}, {2*v}, {1}, {0}}};

    std::array<std::array<Vec3, 4>, 1> temp{};

    matrices::mult<float, Vec3, Vec3, 1, 4, 4>(mu, patch, temp);
    std::array<std::array<Vec3, 1>, 1> point{};
    matrices::mult<Vec3, float, Vec3, 1, 4, 1>(temp, mv, point);

    matrices::mult<float, Vec3, Vec3, 1, 4, 4>(du, patch, temp);
    std::array<std::array<Vec3, 1>, 1> vec_u{};
    matrices::mult<Vec3, float, Vec3, 1, 4, 1>(temp, mv, vec_u);

    matrices::mult<float, Vec3, Vec3, 1, 4, 4>(mu, patch, temp);
    std::array<std::array<Vec3, 1>, 1> vec_v{};
    matrices::mult<Vec3, float, Vec3, 1, 4, 1>(temp, dv, vec_v);

    auto n = vec_u[0][0].cross(vec_v[0][0]).normalize();

    return std::make_pair(point[0][0], n);
}

auto Bezier::calculateCoords() const -> std::vector<Vertex> {
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
    auto patch_points = std::vector<Vec3>(num_points);
    for (size_t i = 0; i < num_points; ++i) {
        float x, y, z;
        file_stream >> x;
        file_stream.ignore();
        file_stream >> y;
        file_stream.ignore();
        file_stream >> z;
        file_stream.ignore();

        patch_points[i] = Vec3::cartesian(x, y, z);
    }

    auto points = std::vector<Vertex>();
    auto f_tess = static_cast<float>(tess);
    for (size_t i = 0; i < num_patches; ++i) {
        auto& curr_patch = patches[i];
        auto patch_mat = Curve::patch_matrix({{
            {patch_points[curr_patch[0]],  patch_points[curr_patch[1]],  patch_points[curr_patch[2]],  patch_points[curr_patch[3]]},
            {patch_points[curr_patch[4]],  patch_points[curr_patch[5]],  patch_points[curr_patch[6]],  patch_points[curr_patch[7]]},
            {patch_points[curr_patch[8]],  patch_points[curr_patch[9]],  patch_points[curr_patch[10]], patch_points[curr_patch[11]]},
            {patch_points[curr_patch[12]], patch_points[curr_patch[13]], patch_points[curr_patch[14]], patch_points[curr_patch[15]]},
        }});
        for (size_t ui = 0; ui < tess; ++ui) {
            auto u = ui / f_tess;
            auto nu = (ui + 1) / f_tess;

            for (size_t vi = 0; vi < tess; ++vi) {
                auto v = vi / f_tess;
                auto nv = (vi + 1) / f_tess;

                auto p0 = get_patch_point(patch_mat, u, v);
                auto p1 = get_patch_point(patch_mat, u, nv);
                auto p2 = get_patch_point(patch_mat, nu, v);
                auto p3 = get_patch_point(patch_mat, nu, nv);

                auto vtx0 = Vertex(p0.first, p0.second, Vec2(u, v));
                auto vtx1 = Vertex(p1.first, p1.second, Vec2(u, nv));
                auto vtx2 = Vertex(p2.first, p2.second, Vec2(nu, v));
                auto vtx3 = Vertex(p3.first, p3.second, Vec2(nu, nv));

                points.push_back(vtx3);
                points.push_back(vtx2);
                points.push_back(vtx1);

                points.push_back(vtx2);
                points.push_back(vtx0);
                points.push_back(vtx1);
            }
        }
    }

    return points;
}
