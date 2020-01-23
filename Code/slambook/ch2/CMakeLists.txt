# declare minimum cmake version reqirement
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# declare a cmake project
#project(helloSLAM VERSION 0.1.0 LANGUAGES CXX and/or C)
project(helloSLAM)

# add executable
add_executable(helloSLAM helloSLAM.cpp)