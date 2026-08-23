cmake_minimum_required(VERSION 3.10)

project(lab3-combined)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main combined.cpp)

target_link_libraries(main OpenGL::GL GLEW::GLEW glfw )