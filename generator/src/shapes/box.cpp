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
    float plane_step = length / n_divisions;
    const short n_faces = 6;
    for(size_t i = 0; i < n_faces; ++i) {
        switch (i) {
            case 1: { // Face base
                Point starting_base = Point(-length / 2, 0, length / 2);
                for (size_t j = 0; j < n_divisions; ++j) {
                    for (size_t k = 0; k < n_divisions; ++k) {
                        Point p1 = starting_base + Point(k * plane_step, 0, j * plane_step);
                        Point p2 = starting_base + Point(k * plane_step, 0, (j + 1) * plane_step);
                        Point p3 = starting_base + Point((k + 1) * plane_step, 0, j * plane_step);
                        Point p4 = starting_base + Point((k + 1) * plane_step, 0, (j + 1) * plane_step);

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            case 2: { // Face do topo
                Point starting_top = Point(-length / 2, length, length / 2);
                for (size_t j = 0; j < n_divisions; ++j) {
                    for (size_t k = 0; k < n_divisions; ++k) {
                        Point p1 = starting_top + Point(k * plane_step, length, j * plane_step);
                        Point p2 = starting_top + Point(k * plane_step, length, (j + 1) * plane_step);
                        Point p3 = starting_top + Point((k + 1) * plane_step, length, j * plane_step);
                        Point p4 = starting_top + Point((k + 1) * plane_step, length, (j + 1) * plane_step);

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            case 3: { // Face traseira direita
                // coordenada do eixo zz mantém-se fixa, coordenada do eixo yy vai aumentando e coordenada do eixo xx vai diminuindo
                Point starting_back_r = Point(length / 2, 0, -length / 2);
                for (size_t j = 0; j < n_divisions; j++) {
                    for (size_t k = 0; k < n_divisions; k++) {
                        Point p1 = starting_back_r + Point(-((j * plane_step)), k * plane_step, length / 2);
                        Point p2 = starting_back_r + Point(-((j + 1) * plane_step), k * plane_step, length / 2);
                        Point p3 = starting_back_r + Point(-(j * plane_step), (k + 1) * plane_step, length / 2);
                        Point p4 = starting_back_r + Point(-((j + 1) * plane_step), (k + 1) * plane_step, length / 2);

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            case 4: { // Face traseira esquerda
                // coordenada do eixo xx mantém-se fixa e coordenada do eixo zz e yy vai aumentando
                Point starting_back_l = Point(-length / 2, 0, -length / 2);
                for (size_t j = 0; j < n_divisions; j++) {
                    for (size_t k = 0; k < n_divisions; k++) {
                        Point p1 = starting_back_l + Point(-length / 2, k * plane_step, j * plane_step);
                        Point p2 = starting_back_l + Point(-length / 2, k * plane_step, (j + 1) * plane_step);
                        Point p3 = starting_back_l + Point(-length / 2, (k + 1) * plane_step, j * plane_step);
                        Point p4 = starting_back_l + Point(-length / 2, (k + 1) * plane_step, (j + 1) * plane_step);

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            case 5: { // Face Frontal Esquerda
                // coordenada do eixo zz mantém-se fixa e coordenada do eixo dos xx e yy vai aumentando
                Point starting_front_l = Point(-length / 2, 0, length / 2);
                for (size_t j = 0; j < n_divisions; j++) {
                    for (size_t k = 0; k < n_divisions; k++) {
                        Point p1 = starting_front_l + Point((j * plane_step), k * plane_step, length / 2);
                        Point p2 = starting_front_l + Point((j + 1) * plane_step, k * plane_step, length / 2);
                        Point p3 = starting_front_l + Point(j * plane_step, (k + 1) * plane_step, length / 2);
                        Point p4 = starting_front_l + Point((j + 1) * plane_step, (k + 1) * plane_step, length / 2);

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            case 6: { // Face frontal direita
                // coordenada do eixo xx mantém-se fixa, coordenada do eixo dos yy vai aumentando e coordenada do eixo dos zz diminuindo
                Point starting_front_r = Point(length / 2, 0, length / 2);
                for (size_t j = 0; j < n_divisions; j++) {
                    for (size_t k = 0; k < n_divisions; k++) {
                        Point p1 = starting_front_r + Point(length / 2, k * plane_step, -(j * plane_step));
                        Point p2 = starting_front_r + Point(length / 2, k * plane_step, -((j + 1) * plane_step));
                        Point p3 = starting_front_r + Point(length / 2, (k + 1) * plane_step, -(j * plane_step));
                        Point p4 = starting_front_r + Point(length / 2, (k + 1) * plane_step, -((j + 1) * plane_step));

                        coords.push_back(p1);
                        coords.push_back(p2);
                        coords.push_back(p3);

                        coords.push_back(p2);
                        coords.push_back(p4);
                        coords.push_back(p3);
                    }
                }
                break;
            }
            default: break;
        }
    }
    return coords;
}