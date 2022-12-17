cmake_minimum_required(VERSION 3.24)
project(game)

set(CMAKE_CXX_STANDARD 17)

link_libraries(/usr/lib/libGL.so.1)
link_libraries(/usr/include/stb/stb_image.h)
link_libraries(/usr/lib/libSOIL.so.1)
link_libraries(glut)

add_executable(game main.cpp)

