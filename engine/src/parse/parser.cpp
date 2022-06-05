#include "parse/parser.hpp"

#include "logger.hpp"
#include "parse/parse_state.hpp"
#include "tinyxml2.h"

#include <fstream>

using namespace tinyxml2;

#define CHECK_NULL(nullable, err)                                              \
    if (!nullable) {                                                           \
        return cpp::fail(err);                                                 \
    }

#define TRY_QUERY_FLOAT(node, name, value, err)                                \
    float value;                                                               \
    if (node->QueryFloatAttribute(name, &value) != XML_SUCCESS) {              \
        return cpp::fail(err);                                                 \
    }

#define TRY_QUERY_INT(node, name, value, err)                                \
    int value;                                                               \
    if (node->QueryIntAttribute(name, &value) != XML_SUCCESS) {              \
        return cpp::fail(err);                                                 \
    }

auto parse_point(XMLElement const* const node) noexcept
    -> cpp::result<Vec3, ParseError>
{
    TRY_QUERY_FLOAT(node, "x", x, ParseError::MALFORMED_POINT);
    TRY_QUERY_FLOAT(node, "y", y, ParseError::MALFORMED_POINT);
    TRY_QUERY_FLOAT(node, "z", z, ParseError::MALFORMED_POINT);

    return Vec3::cartesian(x, y, z);
}

auto parse_projection(XMLElement const* const node) noexcept
    -> cpp::result<Vec3, ParseError>
{
    TRY_QUERY_FLOAT(node, "fov", x, ParseError::MALFORMED_PROJECTION);
    TRY_QUERY_FLOAT(node, "near", y, ParseError::MALFORMED_PROJECTION);
    TRY_QUERY_FLOAT(node, "far", z, ParseError::MALFORMED_PROJECTION);

    return Vec3::cartesian(x, y, z);
}

auto parse_color(XMLElement const* const node) noexcept
    -> cpp::result<Color, ParseError>
{
    TRY_QUERY_INT(node, "R", r, ParseError::MALFORMED_COLOR_COMPONENT);
    TRY_QUERY_INT(node, "G", g, ParseError::MALFORMED_COLOR_COMPONENT);
    TRY_QUERY_INT(node, "B", b, ParseError::MALFORMED_COLOR_COMPONENT);

    return Color::from_rgb(r, g, b);
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
    cpp::result<Vec3, ParseError> up =
        cpp::result<Vec3, ParseError>(Vec3::cartesian(0, 1, 0));
    if (up_elem) {
        up = parse_point(up_elem);
        CHECK_RESULT(up);
    } else {
        warn("no 'up' attribute found for 'camera', using default");
    }

    auto const proj_elem = node->FirstChildElement("projection");
    cpp::result<Vec3, ParseError> proj =
        cpp::result<Vec3, ParseError>(Vec3::cartesian(60, 1, 1000));
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

auto parse_model(XMLElement const* const node, ParseState& parse_state) noexcept
    -> cpp::result<Model, ParseError>
{
    char const* model_file_path;
    node->QueryStringAttribute("file", &model_file_path);
    CHECK_NULL(model_file_path, ParseError::PRIMITIVE_FILE_NOT_FOUND);
    auto model_buffer = parse_state.insert_model_buffer(model_file_path);
    CHECK_RESULT(model_buffer);

    auto maybe_texture_buffer = std::optional<TextureBuffer>();

    char const* texture_file_path;
    auto texture_element = node->FirstChildElement("texture");
    if (texture_element) {
        texture_element->QueryStringAttribute("file", &texture_file_path);
        CHECK_NULL(texture_file_path, ParseError::TEXTURE_FILE_NOT_FOUND);
        auto texture_buffer = parse_state.insert_texture_buffer(texture_file_path);
        CHECK_RESULT(texture_buffer);
        maybe_texture_buffer = *texture_buffer;
    }

    auto color_elem = node->FirstChildElement("color");
    XMLElement const* component;
    auto diffuse = Color::from_rgb(200, 200, 200);
    if (color_elem && (component = color_elem->FirstChildElement("diffuse"))) {
        auto c = parse_color(component);
        CHECK_RESULT(c);
        diffuse = *c;
    }
    auto ambient = Color::from_rgb(50, 50, 50);
    if (color_elem && (component = color_elem->FirstChildElement("ambient"))) {
        auto c = parse_color(component);
        CHECK_RESULT(c);
        ambient = *c;
    }
    auto specular = Color::from_rgb(0, 0, 0);
    if (color_elem && (component = color_elem->FirstChildElement("specular"))) {
        auto c = parse_color(component);
        CHECK_RESULT(c);
        specular = *c;
    }
    auto emissive = Color::from_rgb(0, 0, 0);
    if (color_elem && (component = color_elem->FirstChildElement("emissive"))) {
        auto c = parse_color(component);
        CHECK_RESULT(c);
        emissive = *c;
    }
    auto shininess = 0.0f;
    if (color_elem && (component = color_elem->FirstChildElement("shininess"))) {
        TRY_QUERY_FLOAT(component, "value", c, ParseError::MALFORMED_COLOR_COMPONENT)
        shininess = c;
    }

    return Model(
        *model_buffer,
        maybe_texture_buffer,
        diffuse,
        ambient,
        specular,
        emissive,
        shininess
    );
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

            auto translation_points = std::vector<Vec3>();
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

auto parse_group(XMLElement const* const node, ParseState& parse_state) noexcept
    -> cpp::result<Group, ParseError>
{
    auto models = std::vector<Model>();
    auto const models_elem = node->FirstChildElement("models");
    if (models_elem != nullptr) {
        for (auto model_elem = models_elem->FirstChildElement("model");
             model_elem;
             model_elem = model_elem->NextSiblingElement("model")
        ) {
            auto model = parse_model(model_elem, parse_state);
            CHECK_RESULT(model);
            models.push_back(*model);
        }
    }

    auto groups = std::vector<Group>();
    for (auto group_elem = node->FirstChildElement("group");
         group_elem;
         group_elem = group_elem->NextSiblingElement("group")
    ) {
        auto group_res = parse_group(group_elem, parse_state);
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

auto parse_lights(XMLElement const* const node) noexcept
    -> cpp::result<std::vector<std::unique_ptr<Light>>, ParseError>
{
    auto lights = std::vector<std::unique_ptr<Light>>();

    if (!node) {
        return lights;
    }

    for (auto light_elem = node->FirstChildElement("light");
         light_elem;
         light_elem = light_elem->NextSiblingElement("light")
    ) {
        char const* light_type;
        light_elem->QueryStringAttribute("type", &light_type);
        CHECK_NULL(light_type, ParseError::MALFORMED_LIGHT);

        auto lt = std::string_view(light_type);
        if (lt == "point") {
            TRY_QUERY_FLOAT(light_elem, "posx", px, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "posy", py, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "posz", pz, ParseError::MALFORMED_LIGHT);

            auto light = PointLight::try_new(Vec3::cartesian(px, py, pz));
            CHECK_RESULT(light);
            lights.push_back(std::make_unique<PointLight>(*light));

        } else if (lt == "directional") {
            TRY_QUERY_FLOAT(light_elem, "dirx", dx, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "diry", dy, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "dirz", dz, ParseError::MALFORMED_LIGHT);

            auto light = DirectionalLight::try_new(Vec3::cartesian(dx, dy, dz));
            CHECK_RESULT(light);
            lights.push_back(std::make_unique<DirectionalLight>(*light));

        } else if (lt == "spot") {
            TRY_QUERY_FLOAT(light_elem, "posx", px, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "posy", py, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "posz", pz, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "dirx", dx, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "diry", dy, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "dirz", dz, ParseError::MALFORMED_LIGHT);
            TRY_QUERY_FLOAT(light_elem, "cutoff", cutoff, ParseError::MALFORMED_LIGHT);

            auto light = SpotLight::try_new(
                Vec3::cartesian(px, py, pz),
                Vec3::cartesian(dx, dy, dz),
                cutoff
            );
            CHECK_RESULT(light);
            lights.push_back(std::make_unique<SpotLight>(*light));

        } else {
            return cpp::fail(ParseError::MALFORMED_LIGHT);
        }
    }

    return lights;
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
    auto const lights_element = root->FirstChildElement("lights");

    // parse camera
    auto camera = parse_camera(camera_element);
    CHECK_RESULT(camera);

    // parse groups
    auto parse_state = ParseState();
    auto group = parse_group(group_element, parse_state);
    CHECK_RESULT(group);

    // parse lights
    auto lights = parse_lights(lights_element);
    CHECK_RESULT(lights);

    return World(std::move(*camera), std::move(*group), std::move(*lights));
}
