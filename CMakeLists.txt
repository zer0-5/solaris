cmake_minimum_required(VERSION 3.5)

# Project Name
project(solaris)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_subdirectory(common)
add_subdirectory(libs/tinyxml2)
add_subdirectory(libs/fmt)
add_subdirectory(libs/result)
add_subdirectory(generator)
add_subdirectory(engine)
target_include_directories(common PUBLIC common/include)
target_include_directories(generator PUBLIC generator/include)
target_include_directories(engine PUBLIC engine/include)
