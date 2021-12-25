cmake_minimum_required(VERSION 2.8)

project(chap3Exercise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

if (UNIX)
    include_directories("/usr/include/eigen3")
endif(UNIX)

if (WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../include)  # not recommended
endif(WIN32)

add_executable(chap3Exercise chap3Exercise.cpp)
