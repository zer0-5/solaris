#include "parse/parser.hpp"

#include "logger.hpp"
#include "tinyxml2.h"

#include <fstream>
#include <unordered_map>

using namespace tinyxml2;

#define CHECK_NULL(nullable, err)                                              \
    if (!nullable) {                                                           \
        return cpp::fail(err);                                                 \
    }

#define CHECK_RESULT(result)                                                   \
    if (result.has_error()) {                                                  \
        return cpp::fail(result.error());                                      \
    }

#define TRY_QUERY_FLOAT(node, name, value, err)                                \
    float value;                                                               \
    if (node->QueryFloatAttribute(name, &value) != XML_SUCCESS) {              \
        return cpp::fail(err);                                                 \
    }

auto parse_point(XMLElement const* const node) noexcept
    -> cpp::result<Point, ParseError>
{
    TRY_QUERY_FLOAT(node, "x", x, ParseError::MALFORMED_POINT);
    TRY_QUERY_FLOAT(node, "y", y, ParseError::MALFORMED_POINT);
    TRY_QUERY_FLOAT(node, "z", z, ParseError::MALFORMED_POINT);

    return Point::cartesian(x, y, z);
}

auto parse_projection(XMLElement const* const node) noexcept
    -> cpp::result<Point, ParseError>
{
    TRY_QUERY_FLOAT(node, "fov", x, ParseError::MALFORMED_PROJECTION);
    TRY_QUERY_FLOAT(node, "near", y, ParseError::MALFORMED_PROJECTION);
    TRY_QUERY_FLOAT(node, "far", z, ParseError::MALFORMED_PROJECTION);

    return Point::cartesian(x, y, z);
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
    std::unordered_map<std::string, ModelBuffer>& buffers_map
) noexcept -> cpp::result<Model, ParseError> {
    char const* file_path;
    node->QueryStringAttribute("file", &file_path);

    auto color_elem = node->FirstChildElement("color");
    auto color = Color{1, 1, 1};
    if (color_elem != nullptr) {
        TRY_QUERY_FLOAT(color_elem, "r", r, ParseError::MALFORMED_COLOR);
        TRY_QUERY_FLOAT(color_elem, "g", g, ParseError::MALFORMED_COLOR);
        TRY_QUERY_FLOAT(color_elem, "b", b, ParseError::MALFORMED_COLOR);

        color = Color{r / 255, g / 255, b / 255};
    }

    auto stored_buffer = buffers_map.find(file_path);
    if (stored_buffer == buffers_map.end()) {
        auto buffer = ModelBuffer::try_from_file(file_path);
        CHECK_RESULT(buffer);
        return Model(*buffer, color);
    }

    return Model(stored_buffer->second, color);
}

auto parse_transform(XMLElement const* const node) noexcept
    -> cpp::result<std::unique_ptr<Transform>, ParseError>
{
    auto transform_type = std::string_view(node->Value());

    if (transform_type == "translate") {
        float time;
        if (node->QueryFloatAttribute("time", &time) == XML_SUCCESS) {
            bool align = false;
            node->QueryBoolAttribute("align", &align);

            auto translation_points = std::vector<Point>();
            for (auto point_element = node->FirstChildElement("point");
                point_element;
                point_element = point_element->NextSiblingElement("point")
            ) {
                auto point = parse_point(point_element);
                CHECK_RESULT(point);
                translation_points.push_back(*point);
            }

            return std::make_unique<TimedTranslation>(translation_points, time, align);

        } else {
            auto coords = parse_point(node);
            CHECK_RESULT(coords);

            return std::make_unique<StaticTranslation>(*coords);
        }

    } else if (transform_type == "rotate") {
        auto coords = parse_point(node);
        CHECK_RESULT(coords);

        float time;
        if (node->QueryFloatAttribute("time", &time) == XML_SUCCESS) {
            return std::make_unique<TimedRotation>(time, *coords);

        } else {
            TRY_QUERY_FLOAT(node, "angle", angle, ParseError::MALFORMED_ROTATION);

            return std::make_unique<StaticRotation>(angle, *coords);
        }
    } else if (transform_type == "scale") {
        auto coords = parse_point(node);
        CHECK_RESULT(coords);

        return std::make_unique<Scale>(*coords);
    }

    return cpp::fail(ParseError::UNKNOWN_TRANFORMATION);
}

auto parse_group(
    XMLElement const* const node,
    std::unordered_map<std::string, ModelBuffer>& buffers_map
) noexcept -> cpp::result<Group, ParseError> {
    auto models = std::vector<Model>();
    auto const models_elem = node->FirstChildElement("models");
    if (models_elem != nullptr) {
        for (auto model_elem = models_elem->FirstChildElement("model");
            model_elem;
            model_elem = model_elem->NextSiblingElement("model")
        ) {
            auto model = parse_model(model_elem, buffers_map);
            CHECK_RESULT(model);
            models.push_back(*model);
        }
    }

    auto groups = std::vector<Group>();
    for (auto group_elem = node->FirstChildElement("group");
         group_elem;
         group_elem = group_elem->NextSiblingElement("group")
    ) {
        auto group_res = parse_group(group_elem, buffers_map);
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

    auto buffers_map = new
        std::unordered_map<std::string, ModelBuffer>();
    auto group = parse_group(group_element, *buffers_map);
    delete buffers_map;
    CHECK_RESULT(group);

    return World(std::move(*camera), std::move(*group));
}
