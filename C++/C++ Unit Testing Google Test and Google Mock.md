# C++ Unit Testing: Google Test and Google Mock

[toc]

## Setting Up Google Test

Linking

- Static Linking
  - .a files - Linux
  - .lib files - Windows
- Dynamic Linking
  - .so (shared object) files - Linux
  - .dll (dynamic link library) files - Windows

CMake `FetchContent` allows CMake use get Googletest for us, meaning that we don't need to worry about installing Googletest, building it, copying the libraries and so on.

compile and linking errors

```c++
TEST(TestSuiteName, TestName) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
    
    ASSERT_EQ(3, sqrt(9));
}
```



### Googletest with FetchContent in CMake

`FetchContent_GetProperties(googletest)`, CMake will populate some variables. One of them is `googletest_BINARY_DIR`. More generic, when you use `FetchContent` on some package, the variable will be named `<package-name>_BINARY_DIR`

2 executables, one for the main application, one for running the tests

`FetchContent` was introducted in CMake 3.13, so

```cmake
cmake_minimum_required(VERSION 3.13)
```

headers

```c++
#include <gtest/gtest.h>
```

run_tests.cpp

```c++
#include <gtest/gtest.h>
#include <string>
#include <unistd.h>
#include <glog/logging.h>

int main(int argc, char *argv[]) {
    FLAGS_alsologtostderr = true;
    FLAGS_log_prefix = true;
    FLAGS_max_log_size = 1024;  // in MB
    FLAGS_stop_logging_if_full_disk = true;
    google::InitGoogleLogging("able-tests");

    ::testing::InitGoogleTest(&argc, argv);

    char buff[FILENAME_MAX];  // create string buffer to hold path
    auto res = getcwd(buff, FILENAME_MAX);
    if (res != nullptr) {
        std::string current_working_dir(buff);
        LOG(INFO) << "PWD: " << current_working_dir;
    }

    return RUN_ALL_TESTS();
}
```

CMakeLists.txt

```cmake
# ...
if (BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()

    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/test/run_tests.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/observation/test_able_observation.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/observation/test_plate_recognition.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/route_mapper/test_gps_splitter.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/route_mapper/test_route.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/synced_receiver/test_models_client.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/cut_queue/test_cut_queue.cpp
            )

    # Add test cpp file
    add_executable(able_test ${TEST_SOURCES})

    # Link test executable against gtest & gtest_main
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ../config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    target_link_libraries(able_test able ${EXTERNAL_LIBRARIES} ${INTERNAL_LIBRARIES} gtest pthread ${GLOG_LIB})

    add_test(
            NAME able_test
            COMMAND able_test --gtest_filter=-BuisnessLogic.*:ABLEObservation.*
    )
    add_test(
            NAME observation
            COMMAND able_test --gtest_filter=ABLEObservation.*
    )
    add_test(
            NAME process_event
            COMMAND able_test --gtest_filter=BuisnessLogic.ProcessEvent
    )
    add_test(
            NAME process_batch
            COMMAND able_test --gtest_filter=BuisnessLogic.ProcessBatchEvent
    )
endif ()

```

if `find_package(GTest REQUIRED)` not working

```cmake
# find_package(GTest REQUIRED)

# get googletest
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
# googletest_POPULATED
# googletest_SOURCE_DIR
# googletest_BINARY_DIR
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/helloTest.cpp)

enable_testing()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
# target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} gtest_main gmock_main)
add_test(NAME utils_test COMMAND ${PROJECT_NAME})
```

TODO - the above code is not clean, the main code is not separated from the test code

### Googletest with FetchContent in CMake Visual Studio

Interesting

```cmake
target_link_libraries(${PROJECT_NAME} gtest_main gmock_main)
// works, or also works
target_link_libraries(${PROJECT_NAMR} ${GTEST_LIBRARIES} pthread)
```

or g++ in command line

```bash
g++ -o nonCMakeExe main.cpp -lgtest -lpthread
```

### CMake with a Test Runner and Actual Application

It doesn't make much sense to have the main application as testing

```cmake
cmake_minimum_required(VERSION 3.13)

find_package(GTest REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")

add_library(utility utility.cpp)

add_executable(mainApp main.cpp)
target_link_libraries(mainApp utility)

add_executable(testRunner testRunner.cpp)
target_link_libraries(testRunner utility ${GTEST_LIBRARIES} pthread)
```

```c++
// testRunner.cpp
#include <gtest/gtest.h>
#include "utility.hpp"

TEST(TestSample TestAddition) {
    ASSERT_EQ(2, add(1, 1));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```



## Unit Testing Basics

Unit test is

- automated test
- regression test

Unit test guidelines

- Unit: can execute independently
- run fast
- Do not rely on external inputs

Unit testing + Integration testing + System testing

unit testing is part of functional testing, others are acceptance testing



Unit test structure

- Arrange
- Act
- Assert
- (optional): cleanup phase

code smells

test only one thing at once



convention: the expected value is always the 1st argument, the actual value is the 2nd argument



## Fixtures: Remove Redundant Code

## Setting Up Google Mock

## Google Mock

## Conclusion