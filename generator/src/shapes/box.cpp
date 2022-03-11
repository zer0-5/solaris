#include "shapes/box.hpp"

Box::Box(int argc, char** argv){
    if (argc < 2){
        throw "wrong number of parameters";
    }

    length = std::stof(argv[0]);
    n_divisions = std::stof(argv[1]);
}

std::vector<Point> Box::calculateCoords() const {
    std::vector<Point> coords;
    float const plane_step = length / n_divisions;
    Point starting_point = Point::cartesian(-length/2, - length/2, -length/2);

    for (size_t j = 0; j < n_divisions; ++j) {
        for (size_t k = 0; k < n_divisions; ++k) {

            // Base triangles
            Point p1 = starting_point + Point::cartesian(k * plane_step, 0, j * plane_step);
            Point p2 = starting_point + Point::cartesian(k * plane_step, 0, (j + 1) * plane_step);
            Point p3 = starting_point + Point::cartesian((k + 1) * plane_step, 0, j * plane_step);
            Point p4 = starting_point + Point::cartesian((k + 1) * plane_step, 0, (j + 1) * plane_step);

            coords.push_back(p3);
            coords.push_back(p2);
            coords.push_back(p1);

            coords.push_back(p3);
            coords.push_back(p4);
            coords.push_back(p2);

            // Top triangles
            p1.set_y(length/2); p2.set_y(length/2); p3.set_y(length/2); p4.set_y(length/2);

            coords.push_back(p1);
            coords.push_back(p2);
            coords.push_back(p3);

            coords.push_back(p2);
            coords.push_back(p4);
            coords.push_back(p3);

            // Back Right triangles
            p1 = starting_point + Point::cartesian(k * plane_step, j * plane_step, 0);
            p2 = starting_point + Point::cartesian(k * plane_step, (j + 1) * plane_step, 0);
            p3 = starting_point + Point::cartesian((k + 1) * plane_step, j * plane_step, 0);
            p4 = starting_point + Point::cartesian((k + 1) * plane_step, (j + 1) * plane_step, 0);

            coords.push_back(p1);
            coords.push_back(p2);
            coords.push_back(p3);

            coords.push_back(p2);
            coords.push_back(p4);
            coords.push_back(p3);

            // Front Right triangles
            p1.set_z(length/2); p2.set_z(length/2); p3.set_z(length/2); p4.set_z(length/2);

            coords.push_back(p3);
            coords.push_back(p2);
            coords.push_back(p1);

            coords.push_back(p3);
            coords.push_back(p4);
            coords.push_back(p2);

            // Left Back triangles
            p1 = starting_point + Point::cartesian(0, k * plane_step, j * plane_step);
            p2 = starting_point + Point::cartesian(0, (k + 1) * plane_step, j * plane_step);
            p3 = starting_point + Point::cartesian(0, k * plane_step, (j + 1) * plane_step);
            p4 = starting_point + Point::cartesian(0, (k + 1) * plane_step, (j + 1) * plane_step);

            coords.push_back(p3);
            coords.push_back(p2);
            coords.push_back(p1);

            coords.push_back(p3);
            coords.push_back(p4);
            coords.push_back(p2);

            // Left Front triangles
            p1.set_x(length/2); p2.set_x(length/2); p3.set_x(length/2); p4.set_x(length/2);

            coords.push_back(p1);
            coords.push_back(p2);
            coords.push_back(p3);

            coords.push_back(p2);
            coords.push_back(p4);
            coords.push_back(p3);
        }
    }
    return coords;
}
