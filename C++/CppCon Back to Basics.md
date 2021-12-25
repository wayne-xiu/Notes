# CppCon: Back to Basics

[toc]

## Object-Oriented Programming - 2019

the compiler is your friend and cool kids have friends

advocate modern programming approaches such as *generic programming* and *functional programming*. if use OOP then do it right.

Not all problems breakdown easily into "objects". You can put an "object-oriented" API on almost anything, but that doesn't mean it is a "natural" design fit.

In polymorphism, we are writing code that works with "many" (poly) types. We are writing code without knowing the exact runtime type. 

With OOP we create a base class with virtual functions defining an API. At compile-time, the compiler generates code that calls virtual functions. Like the programmer, the compiler doesn't know the actual execution type, so it can only use the API (including virtual functions) of the object. It cannot, for example, get the actual size of the object or use any type-specific optimizations.

With templated code, we write code against the interface of a concept. At compile-time, the compiler generates the execution code with full knowledge of the actual type of the runtime objects. The power of static polymorphism is this shift in the point of view of the compiler from not knowing anything that compiler doesn't know to know all the details of all the actual types in the executed code.

## Breaking Dependencies: The SOLID Principles - 2020

### **S**ingle-Responsibility

independent, and with a single, well-defined purpose (cohesion) - The pragmatic programmer

decrease coupling

The design of STL follows the SRP; std::vector follows SRP, std::string does not follow the SRP

> Guideline: Prefer cohesive software entities. Everything that does not strictly belong together, should be separated

### **O**pen-Closed Principle

Software artifacts (classes, modules, functions etc.) should be open for extension, but closed for modification

"This kinds of type-based programming has a long history in C, and one of the things we know about it is that it yields programs that are essentially unmaintainable" - Scott Meyers

use virtual method; OCP: an object-oriented approach

> Guideline: Prefer software design that allows the addition of types or operations without need to modify existing code

### **L**iskov Substitution Principle

"Subtypes must be substitutable for their base types."

*Behavioral* subtyping: (aka "IS-A" relationship)

> Guideline: Make sure that inheritance is about behavior, not about data
>
> Guideline: Make sure that the contract of base type is adhered to
>
> Guideline: Make sure to adhere to the required concept

### **I**nterface Segregation Principle

"Clients should not be forced to depend on methods that they do not use"

> Guideline: Make sure interfaces don't induce unnecessary dependencies

### **D**ependency Inversion Principle

"The DIP tells us that the most flexible systems are those in which source code dependencies refer only to abstractions, not to concretions" - Clean Architecture

- High-level modules should not depend on low-level modules. Both should depend on abstractions
- Abstractions should not depend on details. Details should depend on abstractions

Model-view-Controller

> Guideline: Prefer to depend on abstractions (i.e. abstract classes or concepts) instead of concrete types



## Everything You Ever Wanted to Know about DLLs

```c++
#ifdef Using_Lib  // in pre-processor
	#define DECLSPEC _declspec(dllexport)
#else
	#define DECLSPEC _declspec(dllimport)
#endif
```

```c++
class DECLSPEC CRobot {
    ...
};
```

dumpbin

```c++
// export c++ function
extern "C" _declspec(dllexport) void DoSomething();
```

post-build event for DLL project. How to do this in CMake?

## Function and Class Templates - 2019

## Effective CMake - 2017

## Using Modern CMake Patterns to Enforce a Good Modular Design

## Deep CMake for Library Authors - 2019

Craig Scott - CMake maintainer; author of "Professional CMake: A Practical Guide"

### Libraries (mostly shared)

Key questions for library authors

- API Control: what does the library provide?
- Library Consumers: How might the library be used?
- API Compatibility: How does the library evolve?
- Package Maintainers: How might the library be packaged?

API Control

- be clear about what is included in the API (documentation, Headers, Symbol visibility)
- don't expose things that are not part of the API

How to control visibility

Visual studio

```c++
// building
class __declspec(dllexport) MyGenerator {
  public:
    int nextValue();
};
// using
class __declspec(dllimport) MyGenerator {
  public:
    int nextValue();
};
```

we don't want to have two header files

```c++
#include "mytgt_export.h"

class MYTGT_EXPORT MyGenerator {
  public:
    int nextValue();
}
```

```c++
// mytgt_export.h
#ifndef MYTGT_EXPORT
#	ifdef MyTgt_EXPORTS  // building
#		define MYTGT_EXPORT __declspec(dllexport)
#	else				// using
#		define MYTGT_EXPORt __declspec(dllimport)
#	endif
#endif
```

G++/Clang Visibility Control

- Change default visibility to hidden

  ```sh
  -fvisibility=hidden
  ```

- Change visibility of inlined code (including templates)

  ```sh
  -fvisibility-inlines-hidden
  ```

```c++
class __attribute__((visibility("default"))) MyGenerator {
  public:
    int nextValue();
}
```

further

```c++
#include "mytgt_export.h"

class MYTGT_EXPORT MyGenerator {
  public:
    int nextValue();
}
```

```c++
// mytgt_export.h
#ifndef MYTGT_EXPORT
#	define MYTGT_EXPORT __attribute__((visibility("default")))
#endif
```

Now VS, gcc, clang are "unified"



CMake visibility control

```cmake
# set default visibility to hidden for all targets (global setting)
set((CMAKE_CXX_VISIBILITY_PRESET	hidden))
set(CMAKE_VISIBILITY_INLIENS_HIDDEN	Yes)

add_library(MyTgt ...)

include(GeneratorExportHeader)
generate_export_header(MyTgt)
```

TODO

### Cross-platform considerations

### Highlight CMake features

## C++ Unit Testing with Google Test

```c++
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

```c++
TEST(UtilityTest, InitTestCase) {
    // ...
    Robot rbt;
    EXPECT_EQ(rbt.dof, 6);
}
```

## The Forgotten Art of Structured Programming

## Squaring the Circle: Value-oriented design in an object oriented system - 2017

## Design Patterns and Modern C++

## C++ Design Patterns: From C++03 to C++17

## Modern C++ Interfaces - 2017

## OOP is Dead, Long Live Data-oriented Design - 2018

## Data-Oriented Design for Object-Oriented Programmers - 2020

## Object-Oriented Program: Best Practices - 2020

## C++ Code Smells - Jason Turner

## Refactoring C++ Code

## Cross-Platform Pitfalls and How to Avoid them

## Pointers and Memory - 2020

