# Learn C++ from Scratch

[toc]

## Functions and Recursion

main function with parameters

```c++
int main(int argc, char* const argv[])
{
    //...
}
```

parameter option allowing input from the command line;

Passing by value (default behavior), passing by reference/passing by pointer

default parameter; access scope

Recursion

Often, a recursive solution to a problem is very easy to program; the drawback of using recursion is that there is a lot of overhead. Every time a function is called, it is placed in memory

## Pointers & Arrays

Unlike references, pointers are not guaranteed to be *initialized*.

Although static array behaves like a pointer, its value cannot be changed as it references a specific region in memory

accessing array value out of bound is *undefined behavior*

The new operator allocates an object from the heap and optionally initializes it. When you have finished using it, must delete it. Otherwise, the pointed memory is inaccessible and the result is memory leak.

C++ allows us to  create arrays and then use pointers to carry out operation on those arrays

A multidimensional array is not a pointer-to-a-pointer

Pointer to a function

```c++
void (*fp)(float);  // fp is a pointer to a function taking float argument and returns void

void foobar(float fin) {
    //...
}

int main()
{
    fp = &foobar;
	fp(0.5);
    
    return 0;
}

```

## Classes and Inheritance

Independent, self-managing modules and their interactions

use public member functions to access and manipulate private member variables

Constructors

Inherited new class is a specialized version of the existing base class. Inheritance establishes an "is a" relationship between classes

Polymorphism & Virtual Functions

- A virtual function is a member function which is declared in the base class using the keyword virtual and is re-defined (overriden) by the derived class
- The term Polymorphism means the ability to take many forms. It occurs if there is a hierarchy of classes which are all related to each other by inheritance

Without virtual function, the base class pointer to derived class object would only call the function defined in the base. This happens due **static linkage** which means the call is getting set only once by the compiler which is in the base class.

Virtual function is called according to the actual type of the object it referred to

## Templates

Templates are the mechanism by which C++ implements the generic concept. compile time polymorphism

```c++
template<typename Type>
template<class Type>
```

Type is our generic data type's name, and when the template is to be used, it would be the same as if Type was a typedef for your datatype

template classes

When the definition of the member function is outside the class, we have to add the prefix (template<typename T>) for template beforehand (again).

```c++
#include <iostream>
using namespace std;

template <class T>
class Score
{
private:
    T value;
public:
    static int counter;
    Score()  {counter++;}
};

template<class T>
int Score<T>::counter = 0;

int main()
{
    Score<int> x;
    Score<int> y;
    Score<double> z;
    cout << Score<int>::counter<< endl;
    cout << Score<double>::counter<< endl;
    return 0;
}
// output of above code is 2, 1
```





