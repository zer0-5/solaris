#include "parser.hpp"

#include "logger.hpp"
#include "tinyxml2.h"

#include <fstream>
#include <unordered_map>

using namespace tinyxml2;

#define CHECK_NULL(nullable, err)                                              \
    do {                                                                       \
        if (!nullable) {                                                       \
            return cpp::fail(err);                                             \
        }                                                                      \
    } while (0)

#define CHECK_RESULT(result)                                                   \
    do {                                                                       \
        if (result.has_error()) {                                              \
            return cpp::fail(result.error());                                  \
        }                                                                      \
    } while (0)

auto parse_point(XMLElement const* const node) noexcept
    -> cpp::result<Point, ParseError>
{
    float x, y, z;
    if (node->QueryFloatAttribute("x", &x) == XML_SUCCESS
        && node->QueryFloatAttribute("y", &y) == XML_SUCCESS
        && node->QueryFloatAttribute("z", &z) == XML_SUCCESS) {
        return Point::cartesian(x, y, z);
    } else {
        return cpp::fail(ParseError::MALFORMED_POINT);
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
        return cpp::fail(ParseError::MALFORMED_PROJECTION);
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
    cpp::result<Point, ParseError> up =
        cpp::result<Point, ParseError>(Point::cartesian(0, 1, 0));
    if (up_elem) {
        up = parse_point(up_elem);
        CHECK_RESULT(up);
    } else {
        warn("no 'up' attribute found for 'camera', using default");
    }

    auto const proj_elem = node->FirstChildElement("projection");
    cpp::result<Point, ParseError> proj =
        cpp::result<Point, ParseError>(Point::cartesian(60, 1, 1000));
    if (proj_elem) {
        auto proj = parse_projection(proj_elem);
        CHECK_RESULT(proj);
    } else {
        warn("no 'projection' attribute found for 'camera', using default");
    }

    return Camera(
        std::move(*pos),
        std::move(*look_at),
        std::move(*up),
        std::move(*proj)
    );
}

auto parse_model(
    XMLElement const* const node,
    std::unordered_map<std::string, std::shared_ptr<std::vector<Point>>>* points_map
) noexcept -> cpp::result<Model, ParseError> {
    char const* file_path;
    node->QueryStringAttribute("file", &file_path);

    auto stored_points = (*points_map)[file_path];
    if (!stored_points) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            return cpp::fail(ParseError::PRIMITIVE_FILE_NOT_FOUND);
        }
        float x, y, z;
        auto points = std::vector<Point>();
        while (file >> x >> y >> z) {
            points.push_back(Point::cartesian(x, y, z));
        }
        stored_points = std::make_shared<std::vector<Point>>(std::move(points));
    }

    auto color_elem = node->FirstChildElement("color");
    auto color = Color{1, 1, 1};
    if (color_elem != nullptr) {
        float r, g, b;
        if (color_elem->QueryFloatAttribute("r", &r) == XML_SUCCESS
            && color_elem->QueryFloatAttribute("g", &g) == XML_SUCCESS
            && color_elem->QueryFloatAttribute("b", &b) == XML_SUCCESS
        ) {
            color = Color{r / 255, g / 255, b / 255};
        } else {
            return cpp::fail(ParseError::MALFORMED_COLOR);
        }
    }

    return Model(stored_points, color);
}

auto parse_transform(XMLElement const* const node) noexcept
    -> cpp::result<std::unique_ptr<Transform>, ParseError>
{
    auto transform_type = std::string_view(node->Value());

    if (transform_type == "translate") {
        auto coords = parse_point(node);
        CHECK_RESULT(coords);

        return std::make_unique<Translation>(*coords);

    } else if (transform_type == "rotate") {
        float angle;
        if (node->QueryFloatAttribute("angle", &angle) != XML_SUCCESS) {
            return cpp::fail(ParseError::MALFORMED_ROTATION);
        }

        auto coords = parse_point(node);
        CHECK_RESULT(coords);

        return std::make_unique<Rotation>(angle, *coords);

    } else if (transform_type == "scale") {
        auto coords = parse_point(node);
        CHECK_RESULT(coords);

        return std::make_unique<Scale>(*coords);
    }

    return cpp::fail(ParseError::UNKNOWN_TRANFORMATION);
}

auto parse_group(
    XMLElement const* const node,
        std::unordered_map<std::string, std::shared_ptr<std::vector<Point>>>* points_map
) noexcept -> cpp::result<Group, ParseError> {
    auto models = std::vector<Model>();
    auto const models_elem = node->FirstChildElement("models");
    if (models_elem != nullptr) {
        for (auto model_elem = models_elem->FirstChildElement("model");
            model_elem;
            model_elem = model_elem->NextSiblingElement("model")
        ) {
            auto model = parse_model(model_elem, points_map);
            CHECK_RESULT(model);
            models.push_back(*model);
        }
    }

    auto groups = std::vector<Group>();
    for (auto group_elem = node->FirstChildElement("group");
         group_elem;
         group_elem = group_elem->NextSiblingElement("group")
    ) {
        auto group_res = parse_group(group_elem, points_map);
        CHECK_RESULT(group_res);
        groups.push_back(std::move(*group_res));
    }

    auto transforms = std::vector<std::unique_ptr<Transform>>();
    auto transforms_elem = node->FirstChildElement("transform");
    if (transforms_elem != nullptr) {
        for (auto transform_elem = transforms_elem->FirstChildElement();
             transform_elem;
             transform_elem = transform_elem->NextSiblingElement()
        ) {
            auto transform = parse_transform(transform_elem);
            CHECK_RESULT(transform);
            transforms.push_back(std::move(*transform));
        }
    }

    return Group(std::move(models), std::move(groups), std::move(transforms));
}

auto parse(std::string_view file_path) noexcept
    -> cpp::result<World, ParseError>
{
    XMLDocument doc;
    if (doc.LoadFile(file_path.data()) != XML_SUCCESS) {
        return cpp::fail(ParseError::COULD_NOT_OPEN_XML_FILE);
    }

    auto const root = doc.FirstChild();
    auto const camera_element = root->FirstChildElement("camera");
    auto const group_element = root->FirstChildElement("group");

    auto camera = parse_camera(camera_element);
    CHECK_RESULT(camera);

    auto points_map = new
        std::unordered_map<std::string, std::shared_ptr<std::vector<Point>>>();
    auto group = parse_group(group_element, points_map);
    delete points_map;
    CHECK_RESULT(group);

    return World(std::move(*camera), std::move(*group));
}
