# C++ Fundamentals for Professionals

[toc]

## Core Language

C++ 98 was initially more focused on object-oriented programming. With the introduction of C++11, the language shifted focus on *functional programming*.

## Literals

explicit values in the program.

Raw string literals: suppress the interpretation of a string. notice the difference

```c++
#include <iostream>
#include <string>

int main(){

  std::string nat = "C:\temp\newFile.txt";
  std::cout << nat << std::endl;

  // including \t \n
  std::string raw1 = std::string(R"(C:\temp\newFile.txt)");
  std::cout << "\n" << raw1 << std::endl;  

  // raw string including "
  std::string raw3 = std::string(R"(a raw string including ")");
  std::cout << "\n" << raw3 << std::endl;

  return 0;
}

// output:
//C:	emp
//ewFile.txt
//
//C:\temp\newFile.txt
//
//a raw string including "
```



## Types

## Automatic Type Deduction

## Casts

## Unified Initialization

## const, constexpr, and volatile

## Move Semantic and Perfect Forwarding

## Memory Management

## Functions

## Classes and Objects

## Inheritance

## Templates

## Utilities

## Smart Pointers

## Containers in General

## Sequential Containers

## Associative Containers in General

## Ordered Associative Containers

## Unordered Associative Containers

## Algorithms

## Non-Modifying Algorithms

## Modifying Algorithms

## More Algorithms

## Callables

## Iterators

## Strings

## Regular Expressions

## Input and Output

## Threads

## Shared Data

## Tasks

## Conclusion