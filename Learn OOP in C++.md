# Learn Object-Oriented Programming in C++

## Introduction

 **Object-oriented** programming is based on the idea of an `object`. An `object` is an entity with some *data* and *operations*. Data is also referred to as *properties* of the object whereas operations include accessing and modifying those  properties along with other functions that depict the behavior of the  object. 

Concepts of classes, objects, inheritance, dynamic binding and polymorphism

Java and C# are pure Object-oriented language.  In **Java**, even the main function has to be inside a class 

## Functions

Pass-by-value and pass-by-reference

Function overloading: Overloading refers to making a function perform different operations based on the nature of its arguments.

 **Note**: Functions which have no arguments and differ only in the return types cannot be overloaded since the compiler won’t be  able to differentiate between their calls. 

Function overloading plays a vital role in Polymorphism implementation

Q: Find the second minimum in an array

## Pointers

In C++, the stack represents the static section of the RAM. It stores different functions in a stack form (the most recently called function at the top) along with all the variables being used in them.  The members of a function can only be accessed if the said function is at the top of the stack (the function being executed). 

An array name arr can be treated as a pointer where the word arr is the name of a pointer and arr[0] corresponds to *arr (dereferencing)

The dynamic section of memory is known as the heap. *This is a vast space of memory which is not being managed by the CPU*. While the heap does allow us to use as much space as we want, the look-up operation is slower compared to a stack. However, a variable in heap can have a "global" scope instead of just being a local variable. Dynamic memory is allocated at random unlike static memory. Pointer is way to access the heap (new and delete). The pointer itself is stored in the stack but can points to objects in both the stack and the heap.

The *new* objects are created during runtime (instead of compile time). Don't need to specify the amount of memory needed at compile time.

Arrays as input arguments: Since arrays are basically pointers to a block of memory, they are also passed by reference to functions without using the & operator.

Arrays and pointer arithmetic

## Classes

Customized data types; Member variables and member functions;

Access modifiers:

-  private: default
- public: for most member functions
- protected: primary for inheritance implementation

Constructors, no return type; default and parameterized constructor;

this pointer; points to the class object itself

Destructors; C++ does not have transparent garbage collection like Java. To efficiently free memory, we must specify our own destructor for a class. memory leak

Explicit garbage collection with smart pointers

In the case of pointers, destructors are called when issuing the *delete* command

Friend Functions; A friend function is an independent function which has access to the variables and methods of its befriended class.

## Data Hiding

