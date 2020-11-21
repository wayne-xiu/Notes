# Bo Qian - C++

[toc]

## Advanced C++

### const

- A compile time constraint that an object can not be modified

```c++
const int i = 9; 		// i = 6; error

const int* p1 = &i; 	// data is const, pointer is not
int const* p2 = &i;		// equivalent to above

int* const p2; 			// pointer is const, data is not
const int* const p3; 	// data and pointer are both const

// cast
const_cast<int&>(i) = 6; // cast away the const-ness
std::cout << i << std::end;	// still 9

int j;
static_cast<const int&>(j) = 7; // cast j to be const, could not assign value; won't compile
```

- if *const* is on the left of *, data is const
- if *const* is on the right of *, pointer is const
- read from right to left

Why use *const*

- Guards against inadvertent write to the variable (compile time is preferred over runtime)
- Self documenting
- Enables compiler to do more optimization, making code tighter
- const means the variable can be put in ROM (important for embedded programming)

cast is a hacky way of coding, breaking promises and should be avoided generally

### const and Functions



## Modern C++

## C++ Standard Library

## Advanced STL

## Concurrent Programming with C++11

## Boost Library

## C++ Unit Test

## Practical Programming Algorithms

