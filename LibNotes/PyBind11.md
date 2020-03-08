# PyBind11

[toc]

A lightweight header-only library that exposes C++ types in Python and vice versa, mainly to create Python bindings of existing C++ code. Think of this library as a tiny self-contained version of Boost.Python with everything stripped away that isn't relevant for binding generation.

## The Basics

### First steps

#### Bindings for simple function - an example

```c++
#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc = "pybind11 example pluging";  // optional module docstring
    m.def("add", &add, "A function which adds two numbers.");
}
```

- PYBIND11_MODULE: is a macro that creates a function that will be called when an *import* statement is issued from within Python.
- example: is the module name given as the first macro argument (should not be quoted)
- m: defines a variable of type *py::module* which is the main interface for creating bindings.
- module::def: the method generates binding code that exposes the add() function to Python

> all details regarding the function's parameters and return value were automatically inferred *template metaprogramming*.

pybind11 is a header-only library, hence it is not necessary to link against any special libraries and there are no intermediate (magic) translation steps.

```cmake
cmake_minimum_required(VERSION 3.2)
project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(pybind11)
pybind11_add_module(example example.cpp)
```

Building the above C++ code (used cmake) will produce a binary module (like example.cpython-36m-x86_64-linux-gnu.so) file that can be imported to Python. Assuming that the compiled module is located in the current directory, we can load it in Python

```python
import example
example.add(1, 2)  # 3
example.subtract(1, 2)  # -1
```

#### **Keyword arguments**

```c++
// regular notation
m.def("add1", &add, py::arg("i"), py::arg("j"));
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);  // _a suffix is equivalent to arg
```

#### **Default arguments**

Unfortunately, pybind11 cannot automatically extract the default parameter values, since they are not part of the function's type information.

```c++
m.def("add", &add, py::arg("i") = 1, py::arg("j") = 2);
// or
m.def("add", &add, "i"_a = 1, "j"_a = 2);
```

use

```python
import example
example.add()  // 3
```

#### **Exporting variables**

To expose a value from C++, use the *attr* function to register it in a module. Built-in types and general objects are automatically converted when assigned as attributes, and can be explicitly converted using the function py::cast

```c++
m.attr("the_answer") = 42;
py::object world = py::cast("World");
m.attr("what") = world;

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
```

use

```python
import example
example.what  # 'World'
example.the_answer  # 42
example.__version__  # 'dev'
```

#### Supported data types

A large number of data types are supported out of the box and can be used seamlessly as functions arguments, return values or with py::cast in general

### Object-oriented code