#include "shapes/box.hpp"

Box::Box(int argc, char** argv) {
    if (argc < 3) {
        throw "wrong number of parameters";
    }

    length = std::stof(argv[0]);
    n_divisions = std::stof(argv[1]);
}

std::vector<Vertex> Box::calculateCoords() const {
    auto points = std::vector<Vertex>();

    auto nd = static_cast<float>(n_divisions);
    auto starting_point =
        Vec3::cartesian(-length / 2.f, -length / 2.f, -length / 2.f);

    auto n_base = Vec3::cartesian(0, -1, 0);
    auto t_base = Vec2(2/4.f, 0.f);

    auto n_top = Vec3::cartesian(0, 1, 0);
    auto t_top = Vec2(2/4.f, 1.f);

    auto n_right = Vec3::cartesian(0, 0, -1);
    auto t_right = Vec2(3/4.f, 1/3.f);

    auto n_left = Vec3::cartesian(0, 0, 1);
    auto t_left = Vec2(0.f, 1/3.f);

    auto n_back = Vec3::cartesian(-1, 0, 0);
    auto t_back = Vec2(3/4.f, 1/3.f);

    auto n_front = Vec3::cartesian(1, 0, 0);
    auto t_front = Vec2(2/4.f, 1/3.f);

    for (size_t j = 0; j < n_divisions; ++j) {
        for (size_t k = 0; k < n_divisions; ++k) {
            auto i1 = Vec2(k, j) / nd;
            auto i2 = Vec2(k, j + 1) / nd;
            auto i3 = Vec2(k + 1, j) / nd;
            auto i4 = Vec2(k + 1, j + 1) / nd;

            // Base triangles
            auto p1 =
                starting_point + length * Vec3::cartesian(i1.x(), 0, i1.y());
            auto t1 = Vec2(t_base.x() - i1.x() / 4, t_base.y() + i1.y() / 3);

            auto p2 =
                starting_point + length * Vec3::cartesian(i2.x(), 0, i2.y());
            auto t2 = Vec2(t_base.x() - i2.x() / 4, t_base.y() + i2.y() / 3);

            auto p3 =
                starting_point + length * Vec3::cartesian(i3.x(), 0, i3.y());
            auto t3 = Vec2(t_base.x() - i3.x() / 4, t_base.y() + i3.y() / 3);

            auto p4 =
                starting_point + length * Vec3::cartesian(i4.x(), 0, i4.y());
            auto t4 = Vec2(t_base.x() - i4.x() / 4, t_base.y() + i4.y() / 3);

            points.push_back(Vertex(p3, n_base, t3));
            points.push_back(Vertex(p2, n_base, t2));
            points.push_back(Vertex(p1, n_base, t1));

            points.push_back(Vertex(p3, n_base, t3));
            points.push_back(Vertex(p4, n_base, t4));
            points.push_back(Vertex(p2, n_base, t2));

            // Top triangles
            p1.set_y(length / 2);
            t1 = Vec2(t_top.x() - i1.x() / 4, t_top.y() - i1.y() / 3);

            p2.set_y(length / 2);
            t2 = Vec2(t_top.x() - i2.x() / 4, t_top.y() - i2.y() / 3);

            p3.set_y(length / 2);
            t3 = Vec2(t_top.x() - i3.x() / 4, t_top.y() - i3.y() / 3);

            p4.set_y(length / 2);
            t4 = Vec2(t_top.x() - i4.x() / 4, t_top.y() - i4.y() / 3);

            points.push_back(Vertex(p1, n_top, t1));
            points.push_back(Vertex(p2, n_top, t2));
            points.push_back(Vertex(p3, n_top, t3));

            points.push_back(Vertex(p2, n_top, t2));
            points.push_back(Vertex(p4, n_top, t4));
            points.push_back(Vertex(p3, n_top, t3));

            // Right triangles
            p1 = starting_point + length * Vec3::cartesian(i1.x(), i1.y(), 0);
            t1 = Vec2(t_right.x() - i1.x() / 4, t_right.y() - i1.y() / 3);

            p2 = starting_point + length * Vec3::cartesian(i2.x(), i2.y(), 0);
            t2 = Vec2(t_right.x() - i2.x() / 4, t_right.y() - i2.y() / 3);

            p3 = starting_point + length * Vec3::cartesian(i3.x(), i3.y(), 0);
            t3 = Vec2(t_right.x() - i3.x() / 4, t_right.y() - i3.y() / 3);

            p4 = starting_point + length * Vec3::cartesian(i4.x(), i4.y(), 0);
            t4 = Vec2(t_right.x() - i4.x() / 4, t_right.y() - i4.y() / 3);

            points.push_back(Vertex(p1, n_right, t1));
            points.push_back(Vertex(p2, n_right, t2));
            points.push_back(Vertex(p3, n_right, t3));

            points.push_back(Vertex(p2, n_right, t2));
            points.push_back(Vertex(p4, n_right, t4));
            points.push_back(Vertex(p3, n_right, t3));

            // Left triangles
            p1.set_z(length / 2);
            t1 = Vec2(t_left.x() + i1.x() / 4, t_left.y() + i1.y() / 3);

            p2.set_z(length / 2);
            t2 = Vec2(t_left.x() + i2.x() / 4, t_left.y() + i2.y() / 3);

            p3.set_z(length / 2);
            t3 = Vec2(t_left.x() + i3.x() / 4, t_left.y() + i3.y() / 3);

            p4.set_z(length / 2);
            t4 = Vec2(t_left.x() + i4.x() / 4, t_left.y() + i4.y() / 3);

            points.push_back(Vertex(p3, n_left, t3));
            points.push_back(Vertex(p2, n_left, t2));
            points.push_back(Vertex(p1, n_left, t1));

            points.push_back(Vertex(p3, n_left, t3));
            points.push_back(Vertex(p4, n_left, t4));
            points.push_back(Vertex(p2, n_left, t2));

            // Back triangles
            p1 = starting_point + length * Vec3::cartesian(0, i1.x(), i1.y());
            t1 = Vec2(t_back.x() + i1.x() / 4, t_back.y() + i1.y() / 3);

            p2 = starting_point + length * Vec3::cartesian(0, i2.x(), i2.y());
            t2 = Vec2(t_back.x() + i2.x() / 4, t_back.y() + i2.y() / 3);

            p3 = starting_point + length * Vec3::cartesian(0, i3.x(), i3.y());
            t3 = Vec2(t_back.x() + i3.x() / 4, t_back.y() + i3.y() / 3);

            p4 = starting_point + length * Vec3::cartesian(0, i4.x(), i4.y());
            t4 = Vec2(t_back.x() + i4.x() / 4, t_back.y() + i4.y() / 3);

            points.push_back(Vertex(p3, n_back, t3));
            points.push_back(Vertex(p2, n_back, t2));
            points.push_back(Vertex(p1, n_back, t1));

            points.push_back(Vertex(p3, n_back, t3));
            points.push_back(Vertex(p4, n_back, t4));
            points.push_back(Vertex(p2, n_back, t2));

            // Front triangles
            p1.set_x(length / 2);
            t1 = Vec2(t_front.x() - i1.x() / 4, t_back.y() + i1.y() / 3);

            p2.set_x(length / 2);
            t2 = Vec2(t_front.x() - i2.x() / 4, t_back.y() + i2.y() / 3);

            p3.set_x(length / 2);
            t3 = Vec2(t_front.x() - i3.x() / 4, t_back.y() + i3.y() / 3);

            p4.set_x(length / 2);
            t4 = Vec2(t_front.x() - i4.x() / 4, t_back.y() + i4.y() / 3);

            points.push_back(Vertex(p1, n_front, t1));
            points.push_back(Vertex(p2, n_front, t2));
            points.push_back(Vertex(p3, n_front, t3));

            points.push_back(Vertex(p2, n_front, t2));
            points.push_back(Vertex(p4, n_front, t4));
            points.push_back(Vertex(p3, n_front, t3));
        }
    }
    return points;
}
