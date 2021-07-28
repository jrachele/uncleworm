cmake_minimum_required(VERSION 3.19)
project(uncleworm)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
add_executable(uncleworm main.cpp worm.h apples.h level.h state.h global.h)
target_link_libraries(uncleworm sfml-system sfml-graphics)