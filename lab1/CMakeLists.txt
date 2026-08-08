cmake_minimum_required(VERSION 3.10)

project(lab1)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main OpenGL::GL GLEW::GLEW glfw GLUT::GLUT)