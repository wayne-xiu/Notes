cmake_minimum_required(VERSION 2.8)
project(stereoVision)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

# find OpenCV, Pangolin
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

add_executable(stereoVision stereoVision.cpp)
# link OpenCV
target_link_libraries(stereoVision ${OpenCV_LIBS} ${Pangolin_LIBRARIES})
