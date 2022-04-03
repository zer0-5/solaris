#include "parse_error.hpp"

char const could_not_open_xml_file_error_msg[] =
    "error opening XML file";
char const no_camera_position_error_msg[] =
    "no 'position' element found inside 'camera' element";
char const no_camera_look_at_error_msg[] =
    "no 'lookAt' element found inside 'camera' element";
char const no_camera_up_error_msg[] =
    "no 'up' element found inside 'camera' element";
char const no_camera_projecton_error_msg[] =
    "no 'projection' element found inside 'camera' element";
char const malformed_point_error_msg[] =
    "malformed point";
char const malformed_projection_error_msg[] =
    "malformed projection";
char const malformed_rotation_error_msg[] =
    "malformed rotation";
char const unknown_transformation_error_msg[] =
    "unknown transformation";
char const primitive_file_not_found_error_msg[] =
    "primitive file not found";
char const malformed_color_error_msg[] =
    "malformed color";

char const* const error_msg(ParseError e) {
    switch (e) {
    case COULD_NOT_OPEN_XML_FILE:
        return could_not_open_xml_file_error_msg;
    case NO_CAMERA_POSITION_ELEMENT:
        return no_camera_position_error_msg;
    case NO_CAMERA_LOOK_AT_ELEMENT:
        return no_camera_look_at_error_msg;
    case NO_CAMERA_UP_ELEMENT:
        return no_camera_up_error_msg;
    case NO_CAMERA_PROJECTION_ELEMENT:
        return no_camera_projecton_error_msg;
    case MALFORMED_POINT:
        return malformed_point_error_msg;
    case MALFORMED_PROJECTION:
        return malformed_projection_error_msg;
    case MALFORMED_ROTATION:
        return malformed_rotation_error_msg;
    case UNKNOWN_TRANFORMATION:
        return unknown_transformation_error_msg;
    case PRIMITIVE_FILE_NOT_FOUND:
        return primitive_file_not_found_error_msg;
    case MALFORMED_COLOR:
        return malformed_color_error_msg;
    default:
        return "";
    }
}
