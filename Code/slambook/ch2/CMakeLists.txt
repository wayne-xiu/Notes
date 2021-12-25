# declare minimum cmake version reqirement
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# declare a cmake project
#project(helloSLAM VERSION 0.1.0 LANGUAGES CXX and/or C)
project(helloSLAM)

# add executable
add_executable(helloSLAM helloSLAM.cpp)

# add library
add_library(hello_shared libHelloSLAM.cpp)  # static lib
# add_library(hello_shared SHARED libHelloSLAM.cpp)

# add another executable using the library
add_executable(useHello useHello.cpp)
target_link_libraries(useHello hello_shared)