cmake_minimum_required(VERSION 2.8)
project(imageBasics)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

# find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(imageBasics imageBasics.cpp)
# link OpenCV
target_link_libraries(imageBasics ${OpenCV_LIBS})

add_executable(undistortImage undistortImage.cpp)
target_link_libraries(undistortImage ${OpenCV_LIBS})
