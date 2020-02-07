cmake_minimum_required(VERSION 2.8)
project(useSophus)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

if (UNIX)
    include_directories("/usr/include/eigen3")
endif(UNIX)

if (WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../include)  # not recommended
endif(WIN32)

# find Sophus
find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRS})

add_executable(useSophus useSophus.cpp)


add_subdirectory(example)