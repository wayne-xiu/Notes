cmake_minimum_required(VERSION 2.8)
project(ch6NonlinearOptimization)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/camke)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# g2o
#find_package(G2O REQUIRED)
#include_directories(${G2O_INCLUDE_DIRS})

# Eigen
include_directories("/usr/include/eigen3")

add_executable(gaussNewton gaussNewton.cpp)
target_link_libraries(gaussNewton ${OpenCV_LIBS})

add_executable(ceresCurveFitting ceresCurveFitting.cpp)
target_link_libraries(ceresCurveFitting ${OpenCV_LIBS} ${CERES_LIBRARIES})

add_executable(g2oCurveFitting g2oCurveFitting.cpp)
target_link_libraries(g2oCurveFitting ${OpenCV_LIBS} g2o_core g2o_stuff)
