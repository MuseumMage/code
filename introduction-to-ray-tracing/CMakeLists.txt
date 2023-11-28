cmake_minimum_required(VERSION 3.10)
project(introduction-to-ray-tracing VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer raytracer.cpp)
