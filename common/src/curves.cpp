#include "curves.hpp"

#include <math.h>

Curve Curve::catmull_ron(std::vector<Point> points) {
    static const std::array<std::array<float, 4>, 4> catmull_ron_matrix{{
        {-0.5f, +1.5f, -1.5f, +0.5f},
        {+1.0f, -2.5f, +2.0f, -0.5f},
        {-0.5f, +0.0f, +0.5f, +0.0f},
        {+0.0f, +1.0f, +0.0f, +0.0f},
    }};

    return Curve(catmull_ron_matrix, std::move(points));
}

Curve Curve::bezier(std::vector<Point> points) {
    static const std::array<std::array<float, 4>, 4> bezier_matrix{{
        {-1.0f, +3.0f, -3.0f, +1.0f},
        {+3.0f, -6.0f, +3.0f, +0.0f},
        {-3.0f, +3.0f, +0.0f, +0.0f},
        {+1.0f, +0.0f, +0.0f, +0.0f},
    }};

    return Curve(bezier_matrix, std::move(points));
}

auto Curve::get_position(float global_time) const noexcept
    -> std::pair<Point, Point>
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
        Point::cartesian(pv[0], pv[1], pv[2]),
        Point::cartesian(dv[0], dv[1], dv[2]),
    };
}
