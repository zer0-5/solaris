#include "shapes/plane.hpp"

Plane::Plane(int argc, char** argv) {
    if (argc < 2) {
        throw "wrong number of parameters";
    }

    length = std::stof(argv[0]);
    n_divisions = std::stoi(argv[1]);
}

std::vector<Point> Plane::calculateCoords() const {
    std::vector<Point> coords;
    float step = length / n_divisions;
    Point starting_point = Point::cartesian(-length / 2, 0, -length / 2);

    for (size_t i = 0; i < n_divisions; ++i) {
        for (size_t j = 0; j < n_divisions; ++j) {
            Point p1 = starting_point + Point::cartesian(j * step, 0, i * step);
            Point p2 = starting_point + Point::cartesian(j * step, 0, (i + 1) * step);
            Point p3 = starting_point + Point::cartesian((j + 1) * step, 0, i * step);
            Point p4 = starting_point + Point::cartesian((j + 1) * step, 0, (i + 1) * step);

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
