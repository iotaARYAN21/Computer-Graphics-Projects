cmake_minimum_required(VERSION 3.10)

project(lab2)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main triangle.cpp)

target_link_libraries(main OpenGL::GL GLEW::GLEW glfw )