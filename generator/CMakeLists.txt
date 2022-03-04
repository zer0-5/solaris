cmake_minimum_required(VERSION 3.5)

# Project Name
project(generator)

add_executable(${PROJECT_NAME} src/shapes/plane.cpp src/main.cpp)

target_link_libraries(${PROJECT_NAME} common)
