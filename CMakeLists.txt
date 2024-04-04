cmake_minimum_required(VERSION 3.21)

project(varcher VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#include prebuilt fsr from vendor
add_subdirectory(vendor/fsr2)
#include voxel subdirectory
add_subdirectory(vendor/voxel)

add_subdirectory(shader)
add_subdirectory(resource)
add_subdirectory(app)
add_subdirectory(src)