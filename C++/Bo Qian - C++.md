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
std::cout << i << std::endl;	// still 9?

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

*const* used with functions; use when appropriate

```c++
// const parameters
void setAge(const int a) {age = a;}  // this const not very useful without &
// const return value
const string& getName() {return name};
// const function - won't compile if use another non-const function, even without changing anything
void printDogName() const {cout << name << endl;}
// const for overloading - when the object is const will use the const function, if the object is not const, then will call the non-const version
void printDogName() {cout << name << endl;}
```

### Logic Constness and Bitwise Constness

Q. To have pointer to data member and member functions you need to make them public. A. Correct!

What it really means for a function to be *const* logic constness: access an item should be const, however, we are changing counter value; The compiler maintains the concept of bitwise constness.

Solution for conflict: make the counter to be mutable

```c++
mutable int accessCounter;
```

or use const_cast (a hacky way, should be avoided)another conflict with pointer member variable; if the value pointed to by the pointer is modified (logically non-const), the compiler will happily to accept the bitwise constness claim.

use 'mutable' for logical constness

### Compiler Generated Functions

If not explicitly declared: compiler generates

- copy constructor (member by member initialization)
- copy assignment operator (member by member copying)
- destructor
- default constructor (only if there is no constructor declared)
  - call base class's default constructor
  - class data member's default constructor

The STL container requires the contained object being copyable and assignment-able. All reference (as member variable) needs to be initialized and can not be copied through compiler created copy constructor.

C++11 default constructor = default

> - compiler generated functions are public and inline
> - generated only if they are needed

### Disallow Functions



## Modern C++

## C++ Standard Library

## Advanced STL

## Concurrent Programming with C++11

## Boost Library

## C++ Unit Test

## Practical Programming Algorithms

