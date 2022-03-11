#include "parser.hpp"

#include "tinyxml2.h"

#include <fstream>

using namespace tinyxml2;

#define CHECK_NULL(nullable, err)                                              \
    do {                                                                       \
        if (!nullable) {                                                       \
            return cpp::failure(err);                                          \
        }                                                                      \
    }                                                                          \
    while (0)

#define CHECK_RESULT(result)                                                   \
    do {                                                                       \
        if (result.has_error()) {                                              \
            return cpp::failure(result.error());                               \
        }                                                                      \
    }                                                                          \
    while (0)

auto parse_point(XMLElement const* const node) noexcept
    -> cpp::result<Point, ParseError>
{

    float x, y, z;
    if (node->QueryFloatAttribute("x", &x) == XML_SUCCESS &&
        node->QueryFloatAttribute("y", &y) == XML_SUCCESS &&
        node->QueryFloatAttribute("z", &z) == XML_SUCCESS
    ) {
        return Point::cartesian(x, y, z);
    } else {
        return cpp::failure(ParseError::MALFORMED_POINT);
    }
}


auto parse_projection(XMLElement const* const node) noexcept
    -> cpp::result<Point, ParseError>
{
    float x, y, z;
    if (node->QueryFloatAttribute("fov",  &x) == XML_SUCCESS &&
        node->QueryFloatAttribute("near", &y) == XML_SUCCESS &&
        node->QueryFloatAttribute("far",  &z) == XML_SUCCESS
    ) {
        return Point::cartesian(x, y, z);
    } else {
        return cpp::failure(ParseError::MALFORMED_PROJECTION);
    }
}

auto parse_camera(XMLElement const* const node) noexcept
    -> cpp::result<Camera, ParseError>
{
    auto const pos_elem = node->FirstChildElement("position");
    CHECK_NULL(pos_elem, ParseError::NO_CAMERA_POSITION_ELEMENT);
    auto pos = parse_point(pos_elem);
    CHECK_RESULT(pos);

    auto const look_at_elem = node->FirstChildElement("lookAt");
    CHECK_NULL(look_at_elem, ParseError::NO_CAMERA_LOOK_AT_ELEMENT);
    auto look_at = parse_point(look_at_elem);
    CHECK_RESULT(look_at);

    auto const up_elem = node->FirstChildElement("up");
    CHECK_NULL(look_at_elem, ParseError::NO_CAMERA_UP_ELEMENT);
    auto up = parse_point(up_elem);
    CHECK_RESULT(up);

    auto const proj_elem = node->FirstChildElement("projection");
    CHECK_NULL(proj_elem, ParseError::NO_CAMERA_PROJECTION_ELEMENT);
    auto proj = parse_projection(proj_elem);
    CHECK_RESULT(proj);

    return Camera(
        std::move(pos.value()),
        std::move(look_at.value()),
        std::move(up.value()),
        std::move(proj.value())
    );
}

auto parse_model(std::string_view file_path) noexcept
    -> cpp::result<Model, ParseError>
{
    std::ifstream file(file_path.data());
    if (!file.is_open()) {
        return cpp::failure(ParseError::PRIMITIVE_FILE_NOT_FOUND);
    }

    float x, y, z;
    auto points = std::vector<Point>();
    while (file >> x >> y >> z) {
        points.push_back(Point::cartesian(x, y, z));
    }

    return Model(std::move(points));
}

auto parse_group(XMLElement const* const node) noexcept
    -> cpp::result<Group, ParseError>
{
    auto models = std::vector<Model>();
    char const* file_path;

    auto const models_elem = node->FirstChildElement("models");
    CHECK_NULL(models_elem, ParseError::NO_GROUP_MODELS_ELEMENT);

    auto model_elem = models_elem->FirstChildElement("model");
    while (model_elem) {
        model_elem->QueryStringAttribute("file", &file_path);
        auto model = parse_model(file_path);
        CHECK_RESULT(model);
        models.push_back(model.value());
        model_elem = model_elem->NextSiblingElement("model");
    }

    auto groups = std::vector<Group>();

    return Group(std::move(models), std::move(groups));
}

auto parse(std::string_view file_path) noexcept
    -> cpp::result<World, ParseError>
{
    XMLDocument doc;
    if (doc.LoadFile(file_path.data()) != XML_SUCCESS) {
        return cpp::failure(ParseError::COULD_NOT_OPEN_XML_FILE);
    }

    auto const root = doc.FirstChild();
    auto const camera_element = root->FirstChildElement("camera");
    auto const group_element = root->FirstChildElement("group");

    auto camera = parse_camera(camera_element);
    CHECK_RESULT(camera);
    auto group = parse_group(group_element);
    CHECK_RESULT(group);

    return World(std::move(camera.value()), std::move(group.value()));
}
