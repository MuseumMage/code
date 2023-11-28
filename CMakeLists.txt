cmake_minimum_required(VERSION 3.10)
project(scratchapixel VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(introduction-to-ray-tracing)
add_subdirectory(computing-pixel-coordinates-of-3d-point)
add_subdirectory(3d-viewing-pinhole-camera)
add_subdirectory(rasterization-practical-implementation)
