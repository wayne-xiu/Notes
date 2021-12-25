# An Introduction to Modern CMake

[toc]

Notes from this [tutorial book](https://cliutils.gitlab.io/modern-cmake/)

modern CMake (3.18+). Clean, powerful and elegant

Use CMake-like build system to

- avoid hard-coding paths
- use CI
- support different OSs, build package on multiple machines
- support multiple compilers (VS build, Clang, gcc/g++)
- use a library
- use tools like Clang-tidy
- use a debugger

> Your CMake version should be newer than your compiler. It should be newer than the libraries you are using (especially Boost)

```sh
cmake --version
```

## Introduction

### Building a project

Always make a build directory and build from there. Avoid overwriting files or add them to git

```sh
~/package $ mkdir build
~/package $ cd build
~/package/build $ cmake ..
~/package/build $ make
# or for the last line
cmake --build .
```

If using a newer version of CMake, we can run in the package directory directly (don't forget to type the build directory as the argument). You should try to get used to using `--build`, as that will free your from using only `make` to build.

```sh
~/package $ cmake -S . -B build
~/package $ cmake --build build
```

> Note: working from the build directory is historically much more common, and some tools and commands (including CTest) still require running from the build directory.

Any *one* of these commands will install

```sh
# From the build directory (pick one)
~/package/build $ make install
~/package/build $ cmake --build . --target install
~/package/build $ cmake --install . # CMake 3.15+ only

# From the source directory (pick one)
~/package $ cmake --build build --target install
~/package $ cmake --install build # CMake 3.15+ only
```

Just to clarify, you can point CMake at either the source directory *from the build directory* or at an existing build directory from anywhere.

If you use `camke --build` instead of directly calling the underlying build system, you can use

- `-v` for verbose builds
- `-j N` for parallel builds on N cores
- `--target` or `-t` to pick a target

### Picking a compiler

Selecting a compiler must be done on the first run in an empty directory. To pick Clang

```sh
~/package/build $ CC=clang CXX=clang++ cmake ..
```

That sets the environment variables in bash for CC and CXX, and CMake  will respect those variables. This sets it just for that one line, but  that's the only time you'll need those; afterwards CMake continues to  use the paths it deduces from those values.

### Picking a generator

You can build with a variety of tools; `make` is usually the default. To see all the tools CMake knows about on your system, run

```c++
~/package/build $ cmake --help
```

And you can pick a tool with `-G"My Tool"` (quotes only  needed if spaces are in the tool name). You should pick a tool on your  first CMake call in a directory, just like the compiler. Feel free to  have several build directories, like `build/` and `buildXcode`. You can set the environment variable `CMAKE_GENERATOR` to control the default generator (CMake 3.15+). Note that makefiles will only run in parallel if you explicitly pass a number of threads, such as `make -j2`

### Setting Options

You set options in CMake with `-D`

These are common CMake options to most packages:

- `-DCMAKE_BUILD_TYPE=` Pick from Release, RelWithDebInfo, Debug, or sometimes more.
- `-DCMAKE_INSTALL_PREFIX=` The location to install to. System install on UNIX would often be `/usr/local` (the default), user directories are often `~/.local`, or you can pick a folder.
- `-DBUILD_SHARED_LIBS=` You can set this `ON` or `OFF` to control the default for shared libraries (the author can pick one  vs. the other explicitly instead of using the default, though)

### Do's and Don'ts

CMake Antipatterns

- **Do not use global functions**: This includes `link_directories`, `include_libraries`, and similar.
- **Don't add unneeded PUBLIC requirements**: You should avoid forcing something on users that is not required (`-Wall`). Make these PRIVATE instead.
- **Don't GLOB files**: Make or another tool will not know if you add files without rerunning CMake. Note that CMake 3.12 adds a `CONFIGURE_DEPENDS` flag that makes this far better if you need to use it.
- **Link to built files directly**: Always link to targets if available.
- **Never skip PUBLIC/PRIVATE when linking**: This causes all future linking to be keyword-less.

CMake Patterns

- **Treat CMake as code**: It is code. It should be as clean and readable as all other code.
- **Think in targets**: Your targets should represent  concepts. Make an (IMPORTED) INTERFACE target for anything that should  stay together and link to that.
- **Export your interface**: You should be able to run from build or install.
- **Write a Config.cmake file**: This is what a library author should do to support clients.
- **Make ALIAS targets to keep usage consistent**: Using `add_subdirectory` and `find_package` should provide the same targets and namespaces.
- **Combine common functionality into clearly documented functions or macros**: Functions are better usually.
- **Use lowercase function names**: CMake functions and macros can be called lower or upper case. Always use lower case. Upper case is for variables.
- **Use `cmake_policy` and/or range of versions**: Policies change for a reason. Only piecemeal set OLD policies if you have to.

## Basics

### Minimum Version

Here's the first line of every `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.1)
```

a new project should do

```cmake
cmake_minimum_required(VERSION 3.7...3.18)  # range

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
```

### Setting a project

```cmake
project(MyProject VERSION 1.0
                  DESCRIPTION "Very nice sample project"
                  LANGUAGES CXX)  # CXX is default, supports C, CSharp, SWIFT, CUDA etc.
```

### Making an executable

```cmake
add_executable(one two.cpp three.h)
```

- `one` is both the name of the executable file generated, and the name of the CMake target created; Targets show up as folders in many IDEs
- `two.cpp` can list as many as like
- `three.h` The headers will be, for most intents and purposes, ignored; the only reason to list them is to get them to show up in IDEs. 

### Making a library

```cmake
add_library(one STATIC two.cpp three.h)
```

You get to pick a type of library, `STATIC`, `SHARED` or `MODULE`. If leave this choice off, the value of `BUILD_SHARED_LIB` will be used to pick between `STATIC` AND `SHARED`

For a header -only library, nothing needs to be compiled. We need to make a fictional target, that is called an INTERFACE library

### Targets are your friend

```cmake
target_include_directories(one PUBLIC include)
```

adds an include directory to target. `PUBLIC` doesn't mean much for an executable; for a library it lets CMake know  that any targets that link to this target must also need that include directory. Other options are `PRIVATE` (only affect the current target, not dependencies), and `INTERFACE` (only needed for dependencies).

```cmake
# chain targets
add_library(another STATIC another.cpp another.h)
target_link_libraries(another PUBLIC one)  # another needs one
```

`target_link_libraries` is probably the most useful and confusing command in CMake.

- It takes a target (`another`) and adds a dependency if a target is given. If no target of that name (`one`) exits, then it adds a link to a library called `one` on your path (hence the name of the command). Or you can give it a full path to a library. Or a linker flag.

**Focus on using targets everywhere, and keywords everywhere, and you'll be fine**

Targets can have include directories, linked libraries (or linked  targets), compile options, compile definitions, compile features (see  the C++11 chapter), and more. You can often get targets (and always make targets) to represent all the libraries you use. Even things that are not true libraries, like OpenMP, can be represented with targets. This is why Modern CMake is great!

### Dive in

```cmake
cmake_minimum_required(VERSION 3.8)

project(Calculator LANGUAGES CXX)

add_library(calclib STATIC src/calclib.cpp include/calc/lib.hpp)
target_include_directories(calclib PUBLIC include)
target_compile_features(calclib PUBLIC cxx_std_11)

add_executable(calc apps/calc.cpp)
target_link_libraries(calc PUBLIC calclib)
```

### Variables and the Cache

Local variables

```cmake
set(MY_VARIABLE "value")
# access by ${}
${MY_VARIABLE}
```

Cache variables

Properties

### Programming in CMake

```cmake
if(variable)
    # If variable is `ON`, `YES`, `TRUE`, `Y`, or non zero number
else()
    # If variable is `0`, `OFF`, `NO`, `FALSE`, `N`, `IGNORE`, `NOTFOUND`, `""`, or ends in `-NOTFOUND`
endif()
# If variable does not expand to one of the above, CMake will expand it then try again
```

```cmake
if("${variable}")
    # True if variable is not false-like
else()
    # Note that undefined variables would be `""` thus false
endif()
```

generator-expressions - TODO

Macros and Functions - TODO

Arguments

### Communication with your code

Configure File

CMake allows you to access CMake variables from your code using `configure_file`. This command copies a file (traditionally ending in `.in` from one place to another, substituting all CMake variables it finds

Reading files

### How to structure your project

This is what your files should look like when you start a project called `project`, with a library called `lib`, and an executable called `app`

```cmake
- project
  - .gitignore
  - README.md
  - LICENCE.md
  - CMakeLists.txt
  - cmake
    - FindSomeLib.cmake
    - AddGoogleTest.cmake
    - AddPybind.cmake
    - CUDA.cmake
  - include
    - project
      - lib.hpp
  - src
    - CMakeLists.txt
    - lib.cpp
  - apps
    - CMakeLists.txt
    - app.cpp
  - tests
    - CMakeLists.txt
    - testlib.cpp
  - docs
    - CMakeLists.txt
  - extern
    - googletest
  - scripts
    - helper.py
```

- the application folder may be called something else (e.g. bin/) or not exist for a library-only project
- sometime see a `python` folder for python bindings
- or a `cmake` folder for helper CMake files, like `Find<library>.cmake` files

But the basics are there

- `CMakeLists.txt` files are split up over all source directories and are not in the include directories. This is because you should be able to copy the contents of the include directory to `/usr/include` or similar directly (except for configuration headers) and not have any extra files or cause any conflicts. That also why there is a directory for your project inside the include directory

- use `add_subdirectory` to add a subdirectory containing a `CMakeLists.txt`

- You often want a `cmake` folder, with all of your helper modules. This is where your `Find*.cmake` files go. An set of some common helpers is at [github.com/CLIUtils/cmake](https://github.com/CLIUtils/cmake). To add this folder to your CMake path:

  ```cmake
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
  ```

- `extern` folder should contain git submodules almost exclusively. That way, you can control the version of the dependencies explicitly, but still upgrade easily. See the testing chapter for an example of adding a submodule

- should have something like `/build*` in the `.gitignore`, so that users can make build directories in the source directory and use those to build.

- see the [extended code example here](https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project)

### Running other programs

Running a command at configure time

Use `execute_process` to run a process and access the results. It is generally a good idea to avoid hard coding a program path into your CMake. you can use `${CMAKE_COMMAND}`, `find_package(Git)`, or `find_program` to get access to a command to run. Use `RESULT_VARIABLE` to check the return code and `OUTPUT_VARIABLE` to get the output.

Here is an example to updates all git submodules

```cmake
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()
```



Running a command at build time

Build time commands are a bit trickier. The main complication comes from the target system; when do you want your command to run? Does it  produce an output that another target needs? With this in mind, here is an example that calls a Python script to generate a header file:

```cmake
find_package(PythonInterp REQUIRED)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp"
    COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/GenerateHeader.py" --argument
    DEPENDS some_target)

add_custom_target(generate_header ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Generated.hpp DESTINATION include)
```



Included common utilities

A useful tool in writing CMake builds that work cross-platform is `cmake -E <mode>` (seen in CMake files as `${CMAKE_COMMAND} -E`). This mode allows CMake to do a variety of things without calling system tools explicitly, like `copy`, `make_directory`, and `remove`. It is mostly used for the build time commands.

### A simple example

This is a simple yet complete example of a proper CMakeLists. For this  program, we have one library (MyLibExample) with a header file and a  source file, and one application, MyExample,  with one source file.

```cmake
# CMake simple example

## [main]

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.16)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  ModernCMakeExample
  VERSION 1.0
  LANGUAGES CXX)

# If you set any CMAKE_variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

# This is a "default" library, and will match the *** variable setting.
# Other common choices are STATIC, SHARED, and MODULE
# Including header files here helps IDEs but is not required.
# Output libname matches target name, with the usual extensions on your system
add_library(MyLibExample simple_lib.cpp simple_lib.hpp)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(MyExample simple_example.cpp)

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(MyExample PRIVATE MyLibExample)

## [main]

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
enable_testing()
add_test(NAME MyExample COMMAND MyExample)
```

A larger, multi-file example is also [available](https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project)

```cmake
# Works with 3.11 and tested through 3.18
cmake_minimum_required(VERSION 3.11...3.18)

# Project name and a few useful settings. Other commands can pick up the results
project(
  ModernCMakeExample
  VERSION 0.1
  DESCRIPTION "An example project with CMake"
  LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)

  # Docs only available if this is the main app
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14; use our shim
if(${CMAKE_VERSION} VERSION_LESS 3.14)
  include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

# Accumulator library
# This is header only, so could be replaced with git submodules or FetchContent
find_package(Boost REQUIRED)
# Adds Boost::boost

# Formatting library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 5.3.0)
FetchContent_MakeAvailable(fmtlib)
# Adds fmt::fmt

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
```



## Adding Features

This section covers adding common features to CMake project. e.g.

- how to add a variety of options commonly needed in C++ projects, like C++11 support
- how to support IDEs

### Default build type

CMake normally does a "non-release, non debug" empty build type; if you prefer to set the default build type yourself

```cmake
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
```

### C++11 and beyond

CMake 3.8+: Meta compiler feature

```cmake
target_compile_features(myTarget PUBLIC cxx_std_11)  # _11, _14, _17
set_target_properties(myTarget PROPERTIES CXX_EXTENSIONS OFF) # optional
```

CMake 3.1+: Global and property settings

```cmake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

This method isn't bad for a final package, but shouldn't be used by a library. 

> Don't set manual flags yourself. You'll then become responsible for maintaining correct flags for every release of every compiler

### Small but common needs

Position independent code

Little libraries

better to use CMake's generic `find_library`

```cmake
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(MyTarget PUBLIC ${MATH_LIBRARY})
endif()
```

You can pretty easily find `Find*.cmake`'s for this and other libraries that you need with a quick search; most major packages have a helper library of CMake modules

Interprocedural optimization

### CCache and Utilities

CCache - TODO

Utilities

Set the following properties or `CMAKE_*` initializer variables to the command line for the tools. Most of them are limited to C or CXX with make or ninja generators.

- `<LANG>_CLANG_TIDY`: CMake 3.6+
- `<LANG>_CPPCHECK`
- `<LANG>_CPPLINT`
- `<LANG>_INCLUDE_WHAT_YOU_USE`

Clang tidy

This is the command line for running clang-tidy, as a list (remember, a semicolon separated string is a list).

Here is a simple example of using Clang-Tidy:

```cmake
cmake -S . -B build-tidy -DCMAKE_CXX_CLANG_TIDY="$(which clang-tidy);-fix"
cmake --build build -j 1
```

Include what you see - TODO

Link what you see

Clang-format

### Useful modules

- TODO

### Supporting IDEs

In general, IDEs are already supported by a standard CMake project.

open the CMakeLists.txt file from IDE if that IDE has built in support for CMake (CLion, QtCreator etc.)

### Debugging

- TODO



## Including Projects

This is where a good Git system plus CMake shines

### Git Submodule Method

If we want to add a Git repository on the same service (GitHub, GitLab, BitBucket), the following is the correct Git command to set that up as a submodule in the `extern` directory

```sh
$ git submodule add ../../owner/repo.git extern/repo
```

The relative path to the repo is important, it allows you to keep the same access method (ssh or https) as the parent repository. When you are inside the submodule, you can treat it just like a normal repo, and when you are in the parent repository, you can "add" to change the current commit pointer.

But the traditional downside is that you either have to have your users know git submodule commands, or they have to add `--recursive` when they initially clone your repo. CMake can offer a solution

```cmake
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/repo/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
```

- The first line check for Git using CMake's built in FindGit.cmake
- If you are in a git checkout of your source, add an option (defaulting to `ON`) that allows developers to turn off
- We then run the command to get all repositories, and if failed, show a nice error message
- Finally, we verify that the repositories exist before continuing, regardless of the method used to obtain them

you can then include projects that provide good CMake support

```cmake
add_subdirectory(extern/repo)
```

you can build an interface library target yourself if it is a header only project. Or, you can use `find_package` if that is supported, probably preparing the initial search directory  to be the one you've added (check the docs or the file for the `Find*.cmake` file you are using). You can also include a CMake helper file directory if you append to your `CMAKE_MODULE_PATH`, for example to add `pybind11`'s improved `FindPython*.cmake` files.

### Downloading Projects

downloading method: build time

downloading time: configure time

Submodules work so well, that we should discontinue most of the downloads for things like GoogleTest and move them to submodules. Auto downloads will be an issue if we don't have internet access and they are often implemented in the build directory, wasting time and space if you have multiple build directories.

### FetchContent (CMake 3.11+)

Often, you would like to do your download of data or packages as part of the configure instead of the build. It was finally added to CMake itself as part of CMake 3.11 as the FetchContent module

The key ideas are:

- Use `FetchContent_Declare(MyName)` to get data or a package. You can set URLs, Git repositories, and more.
- Use `FetchContent_GetProperties(MyName)` on the name you picked in the first step to get `MyName_*` variables.
- Check `MyName_POPULATED`, and if not populated, use `FetchContent_Populate(MyName)` (and if a package, `add_subdirectory("${MyName_SOURCE_DIR}" "${MyName_BINARY_DIR}")`)

For example, to download catch2

```cmake
cmake_minimum_required(VERSION 3.14...3.18)

project(FetchExample LANGUAGES CXX)

include(FetchContent)
include(CTest)

FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.0)

# CMake 3.14+
FetchContent_MakeAvailable(catch)

add_executable(fetch_example main.cpp)
target_link_libraries(fetch_example PRIVATE Catch2::Catch2)
add_test(NAME fetch_example COMMAND fetch_example)
```

If you can't use CMake 3.14+, the classic way to prepare code was:

```cmake
# CMake 3.11+
FetchContent_GetProperties(catch)
if(NOT catch_POPULATED)
  FetchContent_Populate(catch)
  add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif()
```



## Testing

General testing information

In the main CMakeLists.txt you need to add the following function call (not in a subfolder)

```cmake
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
```

Which will enable testing and set a `BUILD_TESTING` option so users can turn testing on and off. Or you can do this by directly calling `enable_testing()`

When adding test folder, we should do something like this

```cmake
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

The reason for this is that if someone else includes your package, and they use `BUILD_TESTING`, they probably do not want your tests to build.

Building as part of a test - TODO

Testing Frameworks:

- [GoogleTest](https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html): A popular option from Google. Development can be a bit slow.
- [Catch2](https://cliutils.gitlab.io/modern-cmake/chapters/testing/catch.html): A modern, PyTest-like framework with clever macros.
- [DocTest](https://github.com/onqtam/doctest): A replacement for Catch2 that is supposed to compile much faster and be cleaner. See Catch2 chapter and replace with DocTest.

### GoogleTest

#### Submodule method (preferred)

Checkout GoogleTest as a submodule

```sh
git submodule add --branch=release-1.8.0 ../../google/googletest.git extern/googletest
```

Then, in the  main CMakeLists.txt

```cmake
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
```

I would recommend using something like `PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME` to set the default for the `PACKAGE_TESTS` option, since this should only build by default if this is the current project.

Now, in the tests directory:

```cmake
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")
```

If you did this in your main CMakeLists, you could use a normal `add_subdirectory`; the extra path here is needed to correct the build path because we are calling it from a subdirectory.

The next line is optional, but keeps your `CACHE` cleaner:

```cmake
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)
```

If you are interested in keeping IDEs that support folders clean, add these lines

```cmake
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)
```

Then, to add a test, recommend the following macro

```cmake
macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(test1 test1.cpp)
```

#### Download method

Downloading a copy for each project is the recommended way to use  GoogleTest (so much so, in fact, that they have disabled the automatic  CMake install target), so this respects that design decision. This  method downloads the project at configure time, so that IDEs correctly  find the libraries. Using it is simple:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

enable_testing() # Must be in main file

include(AddGoogleTest) # Could be in /tests/CMakeLists.txt
add_executable(SimpleTest SimpleTest.cu)
add_gtest(SimpleTest)
# Note add_gtest is just a macro taht adds gtest, gmock, and gtest_main and then runs add_test to create a test with the same name:
# target_link_libraries(SimpleTest gtest gmock gtest_main)
# add_test(SimpleTest SimpleTest)
```



FetchContent: CMake 3.11 (assuming working on a GitHub repository by using the relative path to googletest)

```cmake
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
```



### Catch

Catch and Catch2 are powerful, idomatic testing solutions similar in philosophy to PyTest for Python. Two ways

1. simply drop in the single include release of Catch into project

```cmake
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")
```

2. If you add the library using ExternalProject, FetchContent, or git submodules, you can also `add_subdirectory` Catch (CMake 3.1+).

## Exporting and Installing

There are three good ways and one bad way to allow others use your library

- Find module (the bad way): don't make a `Find<mypackage>.cmake` script. Use a `Config<mypackage>.cmake` instead

- Add subproject: A package can include your project in a subdirectory, and then use `add_subdirectory` on the subdirectory. This is useful for header-only and quick-to-compile libraries. 
- Exporting: `*Config.camke` scripts

### Installing



### Exporting

### Packaging

## Looking for Libraries (Packages)



# Modern CMake: An Introduction

[link](https://www.youtube.com/watch?v=bDdkJu-nVTo&t=3559s)

header files can be found automatically (in add_executable), and are not usually listed

add_library

The Library Type:

- `STATIC`: static library
- `SHARED`: shared/dynamic library
- `MODULE`: a module (plug-in) library
- If the type is ommited, the `BUILD_SHARED_LIBS` variable will be used to decide between `STATIC` and `SHARED`

```cmake
target_link_libraries(App PUBLIC Cat)
```

- `PUBLIC`: the symbols in `Cat` and any of the symbols in libraries that `Cat` depends on, are visible to `App`
- `target_link_libraries` must come after the target is created
- Can be used for applications, libraries, or other custom targets

Including files and directories

- Use the `add_subdirectory` command to delve into directoiries

- Use the `include` command to add additional CMake scripts

  ```cmake
  add_subdirectory(dir1)  # expects CMakeLists.txt exists in dir1
  include(dir2/HelperScript.cmake)
  ```

- options for add_subdirectory

  ```cmake
  add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])  # [binary] not recommended
  ```

Messages for debugging CMakeLists.txt (`STATUS`, `FATAL_ERROR`)

