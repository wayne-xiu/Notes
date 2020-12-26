# Accelerated Introduction to C++

[toc]

## Introducing C++

using C#, Java to highlight differences; Areas of challenge for "managed" code developers

modern C++

C++ complies to native code, with no virtual machine, ready to be executed

low-level hardware access (raw memory, SIMD, ASM)

### Who uses C++

Key industries

- Games/graphics development
- embedded development
- Quantitative finance

C/C++ used to leverage custom hardware

- CUDA C
- OpenCL for FPGA

### Problems

- Has no Garbage collection mechanism
  - manual memory management
- Lack of metadata
  - Lack of attributes/annotation, proper reflection etc
  - Run-Time Type Information (RTTI) is a poor substitute
- Has a preprocessor
  - Possible source of bugs and ambiguities
- Unreadable Compiler messages
  - Reduces your chances to find out what's wrong and where
- Compilation is slow
  - Cluster builds
- Testing is more difficult
  - no metadata
- 3rd party libraries often shipped as source code
  - you have to compile it in local machine

### Compilers, Build Systems and IDE

Compilers

- MSVC
- GCC - most popular

Build systems

- Local build sytems
  - CMake
  - MSBuild
- Distributed build systems
  - distcc
  - IncrediBuild

IDEs

- Visual Studio - best; ReSharper support
- Qt Creator
  - cross platform
  - part of Qt UI framework

### Libraries 

- STL
- Boost
- Qt Framework - fantastic cross-platform UI development
- Intel libraries (part of Intel Parallel Studio)

## Compilation and Linking

Linking (output types)

Symbol export & interop

entry point; either #include or refer to it explicitly

### Preprocessing

initial compilation stage

- start with # symbol
- C++ functionality is much more powerful
  - preprocessor macros
  - much more dangerous

```c++
// foo() only included in debug mode
#if _DEBUG
void foo() {}
#endif
```

Visual Studio: in C/C++ - Preprocessing - Preprocessor definitions (add your customized)

because lack of metadata, there are a lot of things are done with macros, e.g. testing. Both a blessing and a curse

### Declaration and Definition

```c++
#pragma once  // no need to old #include guard
```

- header files are not compiled

### Compilation

- After preprocessing, all #include statements are implemented

- In other words, each .cpp file has all of its includes included (physically)
  - But not before the includes' include directives
    - Recursively until there are no more include statements

- Each .cpp file becomes one very big file (copy)

  - This is called *translation unit* (.obj)

- Each translation unit is compiled into an object file

  - each can be compiled independently (see the separated compilation thread using IncrediBuild)
  - distributed build systems leverage this fully (different machines, different threads)

### Linking

- Linking joins all the object files together into an executable or library
- In addition to own code, you (probably) want to link to other, external, libraries
- external libraries can be
  - **Static** - the library is included wholesale in the program. There are no additional files to ship. (may bloats the size of your binary)
  - **Dynamic** - the library exists as a separate file
    - **Dynamic-Link library** (.DLL) on Windows, **Shared library** (.SO) on Linux;
    - it has to be shipped (along with the exe)
    - If the library is not available, program will crash
- To link to a library you need
  - Its header files
    - some libraries are header-only, nothing else needed (e.g. Eigen3, some Boost)
  - The actual library (.LIB) files on Windows
  - A dynamic library still needs a library file (.LIB) for linking. Confusing!!
  - Failure to provide libraries result in "unresolved external symbol" errors

Build one our own static library

```c++
// foo.h
#pragma once

int add(int, int);
```

```c++
// foo.cpp
#include "foo.h";
int add(int a, int b) { return a+b; }
```

Compile to StaticLib.lib (file to be linked to)

Client application

```c++
// main.cpp
#include <iostream>
#include <cassert>
#include "../StaticLib/foo.h"

using namespace std;
int main() {
	cout << "2+3 = " << add(2, 3);
    return 0;
}
```

- include the header file
- reference the library for linking (Linker/Input; Linker/General/Additional Library Directories)
- we don't need to ship the .lib file; everything is included



Dynamic library

```c++
#ifdef DYNAMICLIB_EXPORTS
#define DYNAMICLIB_API __declspec(dllexport)
#else
#define DYNAMICLIB_API __declspec(dllimport)
#endif

// define a declaration in header
DYNAMICLIB_API int multiply(int, int);
```

- build will get DynamicLib.dll
  - still need .lib file (DynamicLib.lib)
- Reference the library
  - project property/Linker/Input/Additional Dependencies/DynamicLib.lib

### Symbol Export & Interop

One problem of shipping C++ library is that library has no metadata. Unlike C# and Java, there is no way to just grab the library and be able to tell what kind of functions it exports.

- Exporting a symbol allows a library to advertise its interface
- A dynamic (shared) library has an export table
  - make one yourself
  - use special directives
  - this export table basically contains a list of functions and classes, variables that the library actually contains
  - the consumer of the library can infer the functionalities of that library.
- A utility (DUMPBIN.exe on Windows, nm -D on Linux) can list exported symbols
- Exported symbol names could be mangled
  - This allows us to store additional information about the exported construct
  - Permits things like function overloading
- Exported C++ functions can be consumed from other languages
  - but not classes

build dependencies

```c++
// in command line
dumpbin /exports DynamicLib.dll
// will see symbol mangling
```

```c++
// if we add extern "C", the dumpbin will be cleaner
extern "C"
```



The C# project can consume the C++ dll functions

```c#
[DllImport("DynamicLib.dll", CallingConvention = CallingConvention.Cdecl)]
// declare
public static extern int multiply(int a, int b);
```

Interop Options (Windows here)

- Platform Invocation Services (P/Invoke)
  - Export from C++, [DllImport] in C#, VB
  - Functions only!
- Component Object Model (COM)
  - Cross-language interoperability model
  - still used by Microsoft office and major Windows applications
  - Automation interface, *dynamic* keyword
- Managed C++, C++/CLI
  - a variant of C++ that includes .NET extensions
  - can be used to build a bridge between native and .NET worlds

### Summary

- Preprocessing is powerful
  - but avoid if you can
- each .cpp file gets compiled into an object file
- Object files get linked together, with any external libraries
  - static libraries are included into the final binary
  - dynamic libraries must be shipped with the binary or already available in the sytem
- Project dependencies must be specified *manually* (not like C# or Java)
- Dynamic (shared) libraries export symbols
- Functions exported by such libraries are usable in .NET via P/Invoke

## Basic Syntax

use modern syntax is possible; but sometime its unavoidable to use C syntax

### Integral

- In Java/C#
  - 32-bit signed integer
  - default value 0
- In C++
  - signed integer
  - size if platform-specific
  - default value is unknown

always initialize; use sizeof to find out the size of a scalar type

```c++
size_t a = sizeof(int);
size_t b = sizeof(i);
```

use <cstdint> if possible; which defines integral type of fixed size

### Floating-point types

float - typically single precision (32-bit)

double - typically double precision (64-bit); avoid long double

### Logical types

bool; true or false

warning: numerical values get converted to booleans (avoid doing this)

```c++
// use
if (5 == x)
// instead of 
if (x == 5)
```

### Pointers

- a pointer has an address of a variable
- No default! (as always)
  - initialize
- pointer dereferencing
- compiler performs no checking on pointers

```c++
// declare two pointers
int* a, b;  // wrong
int* a, *b; // good
```

### References

- A reference acts similar to a typical C#/Java reference
  - in C#/Java, there are value types and reference types
  - in C++, reference is not a special type itself, but it works on value types
- No operator to access value
- No concept of "null" reference
- pointer to reference is illegal
  - affects storing references in containers
- Advice: use references instead of pointers if possible
  - especially as function parameters

### Arrays

- C-style arrays are *pointers* to the first element
- int numbers[]{1, 2, 3}
  - we cannot tell from the array how long it is
- int zeros[10]{0};
- When passing an array to a function, you need to pass its length separately
  - stupid
- 0-based
- Multi-dimensional support

```c++
int identity[2][2]{{1, 0}, {0, 1}};
```

- std::array

```c++
std::array<int, 3> values{1, 2, 3};
```

- std::vector

### Character types

- ASCII character stored in a char
  - 1 byte
- wchar_t; unicode
- one the definite weak point of C++

### Strings

- C-style strings
  - array of characters terminated by a zero - The billion dollar mistake
  - char* text = "hello"
- std::string (and wstring)
- provides API for string manipulation
- avoid C-style string if possible

### Summary

- Variables don't have default values, be sure to initialize
- Data type sizes not set in stone
  - use <cstdint> if need portability

## Functions and Variables

### Namespaces and Global scope

pre-compiled header; visual studio pre-compiled these files; pch file

```c++
namespace life
{
    int meaning = 42;
}
using life::meaning;
```

### Functions

```c++
auto add(int a, int b) {
    return a+b;
}
//
auto add(int a, int b) -> int {
    return a+b;
}
```

### Stacks and Heaps

One critical difference between C++ and managed languages: memory management

stack variables get cleaned up

need to clean up heap variables manually

```c++
// stack; size if limited
std::string s("hello");
// heap
std::string* t = new std::string("world");
delete t;
```

use smart pointers if possible, but raw pointer management is not avoidable

### Lambda Functions

functions within functions; anonymous functions

```c++
    int x = 4;
    auto doubleValue = [](int z) {
        return z*2;
    };

    cout << x << "*2 = " << doubleValue(x) << endl;
```

lambda capture

- [] : lambda does not access enclosing scope
- [=] : capture everything by value
- [&] : capture everything by reference
- [x, &y] : capture x by value and y by reference
- [&, z] : capture everything by reference, but z by value

```c++
    int y = 5;
    int x = 4;
    auto increasByY = [&y](int z) {
        return y+z;
    };
    y = 100;  // only works when captured by reference

    cout << x << " incread by " << y << " = " << increasByY(x) << endl;
```

### Enumerations

```c++
// old style; not safe
// Red are exposed directly; 
enum Color {
    Red,
    Green,
    Blue
};
// stronger
enum class Color {
    Red,
    Green,
    Blue
};
```

### Unions

```c++
union Data {
    int integer;
    float fnumber;
    char* text;
};
```

different type of data in the same memory

avoid this

### Structures

a plain data container;

## Control Flow

- For loop (begin/end, iterators, range-based for)
- switch (case fall-through)
- Boolean conversion

C++ switch needs break; this is stupid; C++ can't switch on std::string - stupid

## Object-oriented Programming

### Constants and static members

const is not static by default like in C#

static variable could not be assigned in-line

### Constructors, Destructors

constructor delegation

### Object Copying

copy constructor

memberwise copy by default - shallow copying vs. deep copy (full copy) especially for pointer member variable

copy assignment operator

### Inheritance

static_cast

dynamic_cast throws exceptions for refs, returns nullptr for pointers

### Access Restrictions

public; protected; private (default)

### Virtual Members

base class function re-use

casting references; dynamic_cast for reference and pointer

## Memory Management

### Basic Pointer Ownership

### Rvalues and Move semantics

```c++
int meaningOfLife() { return 42;}
// error
int& x = meaningOfLife();
// rvalue ref is good
int&& y = meaningOfLife();
cout << y << endl;
```

std::move in <memory>

### unique_ptr

```c++
class Address {
public:
	string city_;
	Address() = default;;

	explicit Address(const string& city) : city_(city) {
		cout << city << " created" << endl;
	};

	~Address() {
		cout << city_ << " destroyed" << endl;
	}
};

unique_ptr<Address> CreateAddress(const string& city) {
	return std::make_unique<Address>(city);
}

int main() {
	auto a = CreateAddress("Paris");
	// auto v(a);  // error
    // unique_ptr(const unique_ptr&) = delete;
	// unique_ptr& operator=(const unique_ptr&) = delete;
	return 0;
}
```

### shared_ptr

```c++
class Person {
public:
	shared_ptr<Address> address_;

	explicit Person(const string& city) {
		address_ = std::make_shared<Address>(city);
		cout << "Created Person" << endl;
	}
	~Person() {
		cout << "Destroyed Person" << endl;
	}
};

int main() {
	shared_ptr<Address> a;
	{
		Person p("Paris");
		a = p.address_;
	}  // p is gone
	cout << a->city_ << endl;

	return 0;
}
```



## Templates

generic programming in C++

- Consuming templates
- Template classes
- Template functions
- Template specialization
- variadic Templates
- Metaprogramming

### Consuming templates

```c++
#include <iostream>
#include <tuple>

using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::tuple;

typedef pair<int, int> IntPair;
typedef tuple<int, int, int> Trie;

IntPair sum_and_product(const int a, const int b) {
	return IntPair{ a + b, a*b };
}

Trie sum_product_avg(const int a, const int b, const int c) {
	Trie t{ a + b + c, a*b*c, (a + b + c) / 3 };
	return t;
}

int main() {
	auto a = 2, b = 4;
	auto res = sum_and_product(a, b);
	cout << "sum = " << res.first
		<< " product = " << res.second << endl;

	auto c = 5;
	auto res2 = sum_product_avg(a, b, c);
	cout << "sum = " << std::get<0>(res2) <<
		" product = " << std::get<1>(res2) <<
		" average = " << std::get<2>(res2) << endl;

	return 0;
}
```

note: <tuple> need to be included

notice the use of std::get<>

### Template Classes

```c++
template<typename T1, typename T2, typename T3>
struct Triple {
	T1 first_;
	T2 second_;
	T3 third_;

	Triple(const T1& first, const T2& second, const T3& third) :
		first_(first), second_(second), third_(third) {
	}
};

typedef Triple<int, int, int> trie;
trie sum_product_avg(int a, int b, int c)
{
	return trie{ a + b + c, a*b*c, (a + b + c) / 3 };
}

void consume_template() {
	auto a = 2, b = 4;
	auto c = 5;
	auto res2 = sum_product_avg(a, b, c);

	cout << "sum = " << res2.first_ <<
		" product = " << res2.second_ <<
		" average = " << res2.third_ << endl;
}

int main() {
	consume_template();

	return 0;
}
```

### Template Functions

```c++
template<typename T1, typename T2, typename T3>
Triple<T1, T2, T3> sum_product_avg(const T1& a, const T2& b, const T3& c) {
	return Triple<T1, T2, T3>(a + b + c, a*b*c, (a + b + c) / 3);  // unified initializer {} not working here
}

void template_functions() {
	int a = 14;
	double b = 5.0;
	float c = -3.5f;
	auto result = sum_product_avg(a, b, c);
	cout << "sum = " << result.first_ <<
		" product = " << result.second_ <<
		" average = " << result.third_ << endl;
}

int main() {
	// consume_template();
	template_functions();
	
	return 0;
}
```

### Template Initialization

compile time implementation

```c++
template<typename T1, typename T2, typename T3>
struct Triple {

	typedef T1 result_type;  // assuming result type
	
	T1 first_;
	T2 second_;
	T3 third_;

	Triple(const T1& first, const T2& second, const T3& third) :
		first_(first), second_(second), third_(third) {
	}
};

template<typename T1, typename T2, typename T3>
Triple<T1, T2, T3> sum_product_avg(const T1& a, const T2& b, const T3& c) {
	return Triple<T1, T2, T3>(a + b + c, a*b*c, (a + b + c) / Triple<T1, T2, T3>::result_type(3));
}

void template_functions() {
	int a = 14;
	double b = 5.0;
	float c = -3.5f;
	auto result = sum_product_avg(a, b, c);
	cout << "sum = " << result.first_ <<
		" product = " << result.second_ <<
		" average = " << result.third_ << endl;
}

typedef complex<double> cd;
typedef Triple<cd, cd, cd> cdt;

void template_specialization()
{
	cd a(2, 3), b(3, 4), c(4, 5);  //  complex numbers
	auto result = sum_product_avg(a, b, c);
	cout << "sum = " << result.first_ <<
		" product = " << result.second_ <<
		" average = " << result.third_ << endl;
}

int main() {
	// consume_template();
	template_functions();  // still works for non-complex
	template_specialization();
	
	return 0;
}
```

```c++
typedef complex<double> cd;
typedef Triple<cd, cd, cd> cdt;
// template specialization - with real only
template<> cdt sum_product_avg(const cd& a, const cd& b, const cd& c)
{
	auto result = sum_product_avg(a.real(), b.real(), c.real());
	return cdt(result.first_, result.second_, result.third_);
}

void template_specialization()
{
	cd a(2, 3), b(3, 4), c(4, 5);
	auto result = sum_product_avg(a, b, c);
	cout << "sum = " << result.first_ <<
		" product = " << result.second_ <<
		" average = " << result.third_ << endl;
}

int main() {
	// consume_template();
	template_functions();
	template_specialization();
	
	return 0;
}
```

### Variadic Template

```c++
// variadic templates
template<typename T>
T sum(T t) { return  t; }

// with decltype for return type is not working for some reason in VS2017
template<typename T, typename... U>
auto sum(T t, U... u)/* -> decltype(t + sum(u...))*/
{
	return t + sum(u...);
}

void variadic()
{
	cout << sum(1, 2, 3, 4) << endl;
	cout << sum(string("hello"), string(" world")) << endl;
}

void print() {
	cout << "I am empty function and "
		"I am called at last.\n";
}
// Variadic function Template that takes  
// variable number of arguments and prints 
// all of them. 
template <typename T, typename... Types>
void print(T var1, Types... var2) {
	cout << var1 << endl;
	print(var2...);
}


int main() {
	variadic();
	cout << endl;
	print(1, 2, 3.14, "Pass me any "
		"number of arguments",
		"I will print\n");
	
	return 0;
}
```

### Metaprogramming

in the compile time rather than runtime. This can be abused to perform calculations during compile time, this dark art is "metaprogramming"

```c++
template<int N> struct Factorial
{
	enum {kValue = N * Factorial<N-1>::kValue};
};
template<> struct Factorial<0>
{
	enum {kValue = 1};
};
void metaprogramming()
{
	int x = Factorial<4>::kValue;
	int y = Factorial<0>::kValue;

	cout << "x, y = " << x << " " << y << endl;
}
```

### Summary

- Templates enable generic programming
- Similar to "generics" in C#, Java
  - very similar <> syntax
  - Type parameter prefixed with class or typename
- Compile-time construct
  - Every needed substitution set compiled into the binary
  - can be exploited for computation (aka "template metaprogramming")
- Allow specialization
  - special implementation for particular argument
- Allow literal arguments (i.e. not types)
- can be variadic

## STL

### Containers

```c++
// array
template<int N>
int sum(array<int, N> values)
{
	int result = 0;
	for (int value : values)
		result += value;
	return result;
}

int main() {

	array<int, 5> numbers{ {1, 2, 3, 4, 5} };
	array<int, 3> more_numbers{ {6, 7, 8} };

	cout << sum(numbers) << endl;
	cout << sum(more_numbers) << endl;
	
	return 0;
}
```

vector for index use .at is more safe than []

```c++
// map as hash table
string speech("to be or not to be, that is the question");
map<char, int> histogram;
for (char c: speech)
{
	if (isalpha(c)) histogram[c]++;  // surprising initialization works
}
for (auto u : histogram)
	cout << u.first << " - " << u.second << endl;
```

### Algorithms

```c++
struct Person
{
	string name_;
	int age_;

	friend ostream& operator<< (ostream& os, const Person& p)
	{
		return os << p.name_ << " is " << p.age_ << " years old";
	}
};

int main() {

	vector<Person> people{ {"John", 27}, {"Chris", 32}, {"Ann", 31} };
	auto print_all = [&]()
	{
		cout << people.size() << " person: " << endl;
		for_each(begin(people), end(people),
			[](const Person& p) {cout << p << endl; });
	};

	print_all();

	auto oldest = *max_element(begin(people), end(people),
		[](const Person& a, const Person& b)
		{
		return a.age_ < b.age_;
	});
	cout << "The old person is: " << oldest.name_ << endl;

	// find someone
	auto john = find_if(begin(people), end(people),
		[](const Person& p)
		{
		return p.name_ == string("John");
	});
	if (john != people.end())
		cout << "we found " << john->name_ << " who is " << john->age_ << endl;


	auto youngerThan30 = count_if(people.begin(), people.end(),
		[](const Person& p)
		{
		return p.age_ < 30;
	});
	cout << "Found " << youngerThan30 << " people younger than 30" << endl;


	// sorting
	sort(people.begin(), people.end(),
		[](const Person& a, const Person& b)
		{
		return a.age_ < b.age_;
	});
	cout << "People sorted by age: " << endl;
	print_all();

	// replacement
	Person jill{ "Jill", 44 };
	replace_if(people.begin(), people.end(),
		[](const Person& p)
	{
		return p.name_ == string("John");
	},
		jill);
	print_all();
	
	return 0;
}
```

there are much more algorithms that can be explored

### Streams

<iostream>, <sstream>, <fstream>

### Numerics

```c++
#include <iostream>
#include <string>
#include <chrono>
#include <random>

using namespace std;

int main() {

	cout << "Maximum integer: " << numeric_limits<int>::max() << endl;

	const auto seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	normal_distribution<float> nd;

	for (auto i = 0; i < 10; ++i)
		cout << nd(gen) << "\t";
	
	return 0;
}
```

#END

A C/C++ puzzle:

```c++
// what's the output of this code
int main()
{
    int i = 0;
    int arr[3] = {0};
    for (; i <= 3; i++)
    {
        arr[i] = 0;
        std::cout << "hello world\n";
    }

    return 0;
}
// it will print "hello world" infinitely
// stack from high to low; i-a[2]-a[1]-a[0]; interesting
```

