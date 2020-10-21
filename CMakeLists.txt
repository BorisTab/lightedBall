cmake_minimum_required(VERSION 3.18)
project(lightedBall)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-pthread")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(lightedBall main.cpp vector3.h)

target_link_libraries(lightedBall sfml-graphics sfml-audio)
