#include "shapes/plane.hpp"

Plane::Plane(int argc, char** argv) {
    if (argc < 3) {
        throw "wrong number of parameters";
    }

    length = std::stof(argv[0]);
    n_divisions = std::stoi(argv[1]);
}

std::vector<Vertex> Plane::calculateCoords() const {
    auto points = std::vector<Vertex>();

    auto nd = static_cast<float>(n_divisions);
    auto starting_point = Vec3::cartesian(-length / 2.0f, 0.0f, -length / 2.0f);
    auto n = Vec3::cartesian(0, 1, 0);

    for (size_t i = 0; i < n_divisions; ++i) {
        for (size_t j = 0; j < n_divisions; ++j) {
            auto t1 = Vec2(j, i);
            auto t2 = Vec2(j, i + 1);
            auto t3 = Vec2(j + 1, i);
            auto t4 = Vec2(j + 1, i + 1);

            auto i1 = t1 / nd;
            auto i2 = t2 / nd;
            auto i3 = t3 / nd;
            auto i4 = t4 / nd;

            auto p1 = starting_point + length * Vec3::cartesian(i1.x(), 0, i1.y());
            auto p2 = starting_point + length * Vec3::cartesian(i2.x(), 0, i2.y());
            auto p3 = starting_point + length * Vec3::cartesian(i3.x(), 0, i3.y());
            auto p4 = starting_point + length * Vec3::cartesian(i4.x(), 0, i4.y());

            points.push_back(Vertex(p1, n, t1));
            points.push_back(Vertex(p2, n, t2));
            points.push_back(Vertex(p3, n, t3));

            points.push_back(Vertex(p2, n, t2));
            points.push_back(Vertex(p4, n, t4));
            points.push_back(Vertex(p3, n, t3));
        }
    }
    return points;
}
