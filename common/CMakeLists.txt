cmake_minimum_required(VERSION 3.5)

# Project Name
project(common)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB SRC_FILES src/*.cpp)
add_library(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} fmt)
