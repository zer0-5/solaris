#include "curve.hpp"

#include "matrix.hpp"

#include <math.h>

static const std::array<std::array<float, 4>, 4> catmull_rom_matrix{{
    {-0.5f, +1.5f, -1.5f, +0.5f},
    {+1.0f, -2.5f, +2.0f, -0.5f},
    {-0.5f, +0.0f, +0.5f, +0.0f},
    {+0.0f, +1.0f, +0.0f, +0.0f},
}};

static const std::array<std::array<float, 4>, 4> bezier_matrix{{
    {-1.0f, +3.0f, -3.0f, +1.0f},
    {+3.0f, -6.0f, +3.0f, +0.0f},
    {-3.0f, +3.0f, +0.0f, +0.0f},
    {+1.0f, +0.0f, +0.0f, +0.0f},
}};

auto Curve::catmull_rom(std::vector<Vec3> points) -> Curve {
    return Curve(catmull_rom_matrix, std::move(points));
}

auto Curve::bezier(std::vector<Vec3> points) -> Curve {
    return Curve(bezier_matrix, std::move(points));
}

auto Curve::get_position(float global_time) const noexcept
    -> std::pair<Vec3, Vec3>
{
    auto t = global_time * _points.size();
    auto segment = (int) floor(t);
    t -= segment;

    auto fst_idx = segment + _points.size() - 1;

    auto p1 = _points[(fst_idx + 0) % _points.size()];
    auto p2 = _points[(fst_idx + 1) % _points.size()];
    auto p3 = _points[(fst_idx + 2) % _points.size()];
    auto p4 = _points[(fst_idx + 3) % _points.size()];

    const std::array<std::array<float, 4>, 3> p{{
        {p1.x(), p2.x(), p3.x(), p4.x()},
        {p1.y(), p2.y(), p3.y(), p4.y()},
        {p1.z(), p2.z(), p3.z(), p4.z()},
    }};

    const std::array<float, 4> tv = {t * t * t, t * t, t, 1};
    const std::array<float, 4> tvd = {3 * t * t, 2 * t, 1, 0};

    std::array<float, 3> pv{};
    std::array<float, 3> dv{};

    for (size_t i = 0; i < 3; ++i) {
        std::array<float, 4> a{};

        for (size_t j = 0; j < 4; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                a[j] += p[i][k] * _geometry_matrix[j][k];
            }
        }

        for (size_t j = 0; j < 4; j++) {
            pv[i] += tv[j] * a[j];
            dv[i] += tvd[j] * a[j];
        }
    }

    return {
        Vec3::cartesian(pv[0], pv[1], pv[2]),
        Vec3::cartesian(dv[0], dv[1], dv[2]),
    };
}

auto Curve::patch_matrix(std::array<std::array<Vec3, 4>, 4> points_matrix) noexcept
    -> std::array<std::array<Vec3, 4>, 4>
{
    std::array<std::array<Vec3, 4>, 4> temp{};
    matrices::mult<float, Vec3, Vec3, 4, 4, 4>(bezier_matrix, points_matrix, temp);

    std::array<std::array<Vec3, 4>, 4> res{};
    matrices::mult<Vec3, float, Vec3, 4, 4, 4>(temp, bezier_matrix, res);

    return res;
}
