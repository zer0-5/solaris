#pragma once

#include <string>

#define CHECK_RESULT(result)                                                   \
    if (result.has_error()) {                                                  \
        return cpp::fail(result.error());                                      \
    }

enum ParseError : char {
    COULD_NOT_OPEN_XML_FILE,
    NO_CAMERA_POSITION_ELEMENT,
    NO_CAMERA_LOOK_AT_ELEMENT,
    NO_CAMERA_UP_ELEMENT,
    NO_CAMERA_PROJECTION_ELEMENT,
    MALFORMED_POINT,
    MALFORMED_PROJECTION,
    MALFORMED_ROTATION,
    UNKNOWN_TRANFORMATION,
    PRIMITIVE_FILE_NOT_FOUND,
    MALFORMED_COLOR_COMPONENT,
    MALFORMED_LIGHT,
    LIGHT_LIMIT,
    TEXTURE_FILE_NOT_FOUND
};

char const* const error_msg(ParseError);
