cmake_minimum_required(VERSION 3.5)

# Project Name
project(generator)

file(GLOB SRC_FILES src/shapes/*.cpp src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} common fmt)
