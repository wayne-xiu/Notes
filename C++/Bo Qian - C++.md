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

as long as we define a constructor taking any argument, the compiler will not generate the default constructor.

need to disallow: two handles write to the same file; to disallow in C++11, using delete; In C++03, we can make the copy constructor to be private and giving copy constructor only declaration but no definition.

The same can be used for copy assignment operator

```c++
class OpenFile {
public:
    OpenFile(string filename) {cout << "Open a file " << filename << endl;}
    OpenFile(OpenFile& rhs) = delete;
    OpenFile &operator=(const OpenFile &rhs) = delete;
// C++03;
private:
    // OpenFile(OpenFile& rhs);
};
```

Making destructor being private is not a good idea.

friends are worse than enemies.

reference counting shared pointers; count the number of pointers pointing to itself

we have to provide a public interface for destruction

```c++
class OpenFile
{
public:
    OpenFile(string filename) { cout << "Open a file " << filename << endl; }
    void destroyMe() { delete this; }

private:
    ~OpenFile() { cout << "OpenFile destructed." << endl; }
};

int main()
{
    // not working
    // OpenFile f(string("_file"));
    // f.destroyMe();
    OpenFile *f = new OpenFile(string("_file"));
    f->destroyMe();
}
```

If f is on stack, it's still not working.

Conclusion:

- A class with private destructor can only be stored on *heap* but not *stack*
- this can be useful for embedded programming, when the stack is small, easily cause stack overflow; we can store heavy class object on heap with private destructor; this can be forced with private destructor

Summary of disallowing functions:

- C++11: f() = delete;
- C++03: declare a function to be private, and not define it
- Private destructor: stay out of stack

### Virtual Destructors and Smart Destructor

When an object is used polymorphically, how to make sure the correct destructor is invoked in the end?

```c++
class Dog
{
public:
    ~Dog() { cout << "Dog destroyed" << endl; }
};

class YellowDog : public Dog
{
public:
    ~YellowDog() { cout << "Yellow dog destroyed" << endl; }
};

class DogFactory
{
public:
    static Dog *createYellowDog() { return (new YellowDog()); }
    //... create other dogs
};

int main()
{
    Dog *pd = DogFactory::createYellowDog();
    //... do something with pd

    delete pd;
    return 0;
}
```

output of above code

```sh
Dog destroyed
```

with

```c++
class Dog
{
public:
    virtual ~Dog() { cout << "Dog destroyed" << endl; }
};
```

the output will be

```sh
Yellow dog destroyed
Dog destroyed
```

**If a class is meant to be used in a polymorphic way, then the base class must have a virtual destructor.**



If we really don't want to have a destructor, there is option 2: - shared_ptr

```c++
class Dog
{
public:
    ~Dog() { cout << "Dog destroyed" << endl; }  // don't need virtual destructor
};

class YellowDog : public Dog
{
public:
    ~YellowDog() { cout << "Yellow dog destroyed" << endl; }
};

class DogFactory
{
public:
    static shared_ptr<Dog> createYellowDog()
    {
        return shared_ptr<YellowDog>(new YellowDog());
    }
    //... create other dogs
};

int main()
{
    shared_ptr<Dog> pd = DogFactory::createYellowDog();
    //...

    // delete pd;
    return 0;
}
```

output:

```sh
Yellow dog destroyed
Dog destroyed
```

unique_ptr won't work this way

Note: *All classes in STL have no virtual destructor*, so be careful inheriting from them. One solution is to use shared_ptr as much as possible

### Exceptions in Destructors

Don't allow exceptions to escape from destructor, because the result could be disastrous.



## Modern C++

## C++ Standard Library

## Advanced STL

## Concurrent Programming with C++11

## Boost Library

## C++ Unit Test

## Practical Programming Algorithms

