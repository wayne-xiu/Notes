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



