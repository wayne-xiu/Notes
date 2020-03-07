# Learn Object-Oriented Programming in C++

[toc]

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

Data hiding is an essential process in the OOP cycle.

In layman’s terms, data hiding refers to the concept of **hiding the inner workings of a class** and simply providing an **interface** through which the outside world can interact with the class without knowing what’s going on inside.

Data hiding can be divided into two primary components:

1. Encapsulation
2. Abstraction

![dataHiding](Media/dataHiding.png)

Abstraction focuses on revealing only the relevant parts of the application while keeping the inner implementation hidden.

abstraction using classes; abstraction using header files

## Inheritance

HAS-A and IS-A relationship

```c++
class derivedClassName : modeOfInheritance baseClassName
    // mode: public, private, protected
```

Protected and public data members are accessible to derived classes

Construction order: base -> derived;

Destruction order: derived -> base

![modesOfInheritance](Media/modesOfInheritance.png)

Multiple inheritance and multilevel inheritance

```c++
class Honda: public Vehicle, public Cars
```

```c++
class Cars: public Vehicle
class Honda: public Cars
```

## Polymorphism

exhibits many forms or behaviors; overriding

Polymorphism only works with a pointer and reference types (a base class pointers points to the derived class objects)

A virtual function is a member function which is declared within the base class and is *overridden* by the derived class. When referring to a derived class object using a pointer or a reference to the base class, we can call a virtual function for that object and execute the derived class's version of the function. Virtual functions ensure the correct function is called for an object. They are mainly used to achieve Runtime polymorphism (template achieves Compile time polymorphism)

abstract class and pure virtual function

## Composition, Aggregation and Association

OOD (object-oriented design) deals with the use of different class objects to create the design of an application.

Composition, aggregation and association are used to link different classes together through a variety of relationships. 

- Part-of (dependent)
- Has-a (independent with reference)

![part-of](Media/part-of.png)

![has-a](Media/has-a.png)

Composition is accessing other classes objects in your class and owner class owns the object and is responsible for its lifetime. Composition relationships are Part-of relationships

> Note: In composition, the lifetime of the owned object depends on the lifetime of the owner

Aggregation follows the Has-a model.

> Note: In aggregation, the lifetime of the owned object does not depend on the lifetime of the owner

In composition, the parent **contains** a child object. This bounds the child to its parent. In aggregation, the parent only contains a **reference** to the child, which removes the child’s dependency (a looser relationship, e.g. people dies, but county still exists).

Association is the relationship between the unrelated objects of the classes. Example is student and teacher objects sharing a connection but can exist independently. Association is Has-a relationship



