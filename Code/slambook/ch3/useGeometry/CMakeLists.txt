cmake_minimum_required(VERSION 2.8)

project(geometry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

include_directories("/usr/include/eigen3")

add_executable(useGeometry useGeometry.cpp)
