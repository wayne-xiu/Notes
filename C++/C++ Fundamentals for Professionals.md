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

user-defined literals for type safety (with corresponding literal operator)

![Cpp14Literals](Media/Cpp14Literals.png)

In C++14, string literals are supported. C++ used to only support C-string literals, meaning that we had to always use a C-string literal to initialize a C++ string. Time literals are very convenient; time literals are of type std::chrono::duration

```c++
#include <iostream>
#include <ostream>

namespace Distance{
  class MyDistance{
    public:
      MyDistance(double i):m(i){}

      friend MyDistance operator +(const MyDistance& a, const MyDistance& b){
        return MyDistance(a.m + b.m);
      }
      friend MyDistance operator -(const MyDistance& a, const MyDistance& b){
        return MyDistance(a.m - b.m);
      }

      friend std::ostream& operator<< (std::ostream &out, const MyDistance& myDist){
        out << myDist.m << " m";
         return out;
      }
    private:
      double m;

  };

  namespace Unit{
    MyDistance operator "" _km(long double d){
      return MyDistance(1000*d);
    }
    MyDistance operator "" _m(long double m){
      return MyDistance(m);
    }
    MyDistance operator "" _dm(long double d){
      return MyDistance(d/10);
    }
    MyDistance operator "" _cm(long double c){
      return MyDistance(c/100);
    }
    MyDistance operator "" _ft(long double ft) {
      return MyDistance(ft*0.348);
    }
    MyDistance operator "" _mi(long double mi) {
      return MyDistance(mi*1609.344);
    }
  }
}

using namespace Distance::Unit;

int main(){

  std:: cout << std::endl;

  std::cout << "1.0_km: " << 1.0_km << std::endl;
  std::cout << "1.0_m: " << 1.0_m << std::endl;
  std::cout << "1.0_dm: " << 1.0_dm << std::endl;
  std::cout << "1.0_cm: " << 1.0_cm << std::endl;
  std::cout << "1.0_ft: " << 1.0_ft << std::endl;
  std::cout << "1.0_mi: " << 1.0_mi << std::endl;

  std::cout << std::endl;
  std::cout << "1.0_km + 2.0_dm +  3.0_dm - 4.0_cm: " << 1.0_km + 2.0_dm +  3.0_dm - 4.0_cm << std::endl;
  std::cout << std::endl;

  return 0;
}


// output:
// 1.0_km: 1000 m
// 1.0_m: 1 m
// 1.0_dm: 0.1 m
// 1.0_cm: 0.01 m
// 1.0_ft: 0.348 m
// 1.0_mi: 1609.34 m
//
// 1.0_km + 2.0_dm +  3.0_dm + 4.0_cm: 1000.54 m
```

- should use namespaces for user-defined literals for collision avoidance.

## Types

Drawbacks of enumerations in classical C++

- Enumerators implicit convert to int
- Enumerators in the enclosing scope
- The type of the enumeration cannot be specified

Strongly-type enumerations since C++11, enum class/ enum struct, which

- can only be accessed in the scope of the enumeration
- don't implicitly convert to int (can use static_cast<int>(NewEnum::one))



**Pointers**

A pointer holds the address of a value. pointer dereferencing; create dynamic data using pointers, with *new* keyword.

Use nullptr (type std::nullptr_t) instead of number 0 or macro NULL

- we can assign nullptr to arbitrary pointers
- a nullptr can be compared with all other pointers

type deduction in template function will deduce 0 to int, NULL to type long int, instead of pointer type. The nullptr cannot be assigned to any arbitrary variable except a bool (through uniform initialization)

Function pointers

```c++
void addOne(int& x){
  x += 1;
}
void (*inc1)(int&)= addOne;  // function pointer
auto inc2 = addOne;

```

Pointer to struct/class member

reference: alias. A reference is never NULL and must always be initialized by having an existing variable assigned to it.

References behave like constant pointers

## Automatic Type Deduction

### auto

"auto" introduced since C++11

```c++
  // define a pointer to a function
  int (*add)(int,int)= myAdd;               // explicit
  auto add1= myAdd;                         // auto
  
  // iterate through a vector
  std::vector<int> vec;
  for (std::vector<int>::iterator it= vec.begin(); it != vec.end(); ++it){} 
  for (auto it1= vec.begin(); it1 != vec.end(); ++it1) {}
```



auto determines its type from an initializer. That simply means that; without an initializer, there is no type and therefore, no variable. The compiler ensures that each type is initialized, which is a nice side effect of auto (avoid undefined behavior).

> default initialization (the initialization performed when a variable is constructed with no initializer):
>
> “objects with automatic storage duration (and their sub-objects) are initialized to indeterminate values”
>
> Local variables that are not user-defined will not be default initialized.

Refactorization with auto

```c++
#include <typeinfo>  // for using typeid
#include <iostream>

int main(){
  auto a = 5;
  auto b = 10;
  auto sum =  a * b * 3;
  auto res = sum + 10; 
  std::cout << "typeid(res).name(): " << typeid(res).name() << std::endl;
  
  auto a2 = 5;
  auto b2 = 10.5;
  auto sum2 = a2 * b2 * 3;
  auto res2 = sum2 * 10;  
  std::cout << "typeid(res2).name(): " << typeid(res2).name() << std::endl;
  
  auto a3 = 5;
  auto b3 = 10;
  auto sum3 = a3 * b3 * 3.1f;
  auto res3 = sum3 * 10;  
  std::cout << "typeid(res3).name(): " << typeid(res3).name() << std::endl;
  
  return 0;
}

// output:
// typeid(res).name(): i
// typeid(res2).name(): d
// typeid(res3).name(): f
```

auto combines the dynamic behavior of an interpreter with the static behavior of a compiler

Advanced types

```c++
#include <chrono>
#include <future>
#include <map>
#include <string>
#include <tuple>

int main(){
  // define a function pointer
  int (*myAdd1)(int, int) = [](int a, int b){return a + b;};
  // use type inference of the C++11 compiler
  auto myAdd2 = [](int a, int b){return a + b;};
    
  auto myInts = {1, 2, 3};  // initializer_list<int>
  auto myIntBegin = myInts.begin();  // initializer_list<int>::iterator

  std::map<int, std::string> myMap = {{1, std::string("one")}, {2, std::string("two")}};
  auto myMapBegin = myMap.begin();  // map<int, string>::iterator

  auto func = [](const std::string& a){ return a;};  // (string)(*)(string)?
    							  // std::function<string(const string&)>

  auto futureLambda= std::async([](const std::string& s ) {return std::string("Hello ") + s;}, std::string("lambda function."));  // std::future<string>

  auto begin = std::chrono::system_clock::now();  // chrono::time_point<chrono::system_clock>

  auto pa = std::make_pair(1, std::string("second"));  // pair<int, string>

  auto tup = std::make_tuple(std::string("first"), 4, 1.1, true, 'a');  // tuple<string, int, double, bool, char>
    
    return 0;
}
```

### decltype

decltype is used to determine the type of an expression or entity

```c++
	int i = 1998; // Rvalue
	decltype(i) i2 = 2011; // Same as int i2 = 2011
	decltype((i)) iRef = i2; // (i) is an lvalue, reference returned

	cout << typeid(i2).name() << endl;
	cout << typeid(iRef).name() << endl;  // vs2017 showed int, should be int&
```

Rules:

- if the expression is an *lvalue*, decltype will return **a reference to the data type to the expression**
- if the expression is an rvalue, decltype will return **the data type of the value**

decltype is not used as often as auto. It is useful with templates that can deduce the type of a function (or function pointer).

### Automatic Return type

C++14

```c++
template <typename T1, typename T2>
auto add(T1 fir, T2 sec) /*-> decltype(fir+sec)*/{
    return fir + sec;
}
```



## Casts

### Explicit Cast

- dynamic_cast
- static_cast
- const_cast
- reinterpret_cast

We should avoid using C-casts ((type) value_to_be_casted)

*explicit is better than implicit*

### Dynamic Cast

- dynamic_cast converts a pointer or reference of a class to a pointer or reference in the same inheritance hierarchy. 
- It can only be used with polymorphic classes. We can cast up, down and across the inheritance hierarchy. dynamic_cast is mostly used when converting from a derived class to a base class (upcast)
- Type information at run time is used to determine if the cast is valid. 
- If the cast is not possible, we will get a nullptr in case of pointer and an std::bad_cast-exception in case of a reference

> Do keep in mind that dynamic_cast only deals with pointers and references

### Static Cast

- static_cast is the simplest casting operator and is used for simple conversions. 
- It can only perform well-defined conversions by the compiler.
- It allows bidirectional conversion between related data types such as: pointer types in class hierarchies
- static_cast cannot be used with polymorphic types
- unlike dynamic_cast, a static_cast is performed during compile time

### Const Cast

- const_cast allows us to remove or add the const or volatile property from a variable
- rarely used

### Reinterpret Cast

- reinterpret_cast allows us to convert a pointer of a particular type to a pointer of any other type regardless of whether the types are related or not
- It also allows conversion between a pointer and an integral
- reinterpret_cast guarantees that if a pointer is cast into another type, casting it back would return the original value
- not recommended

### Type Information

```c++
#include <typeinfo>

Circle c(5.0);
const std::type_info& t = typeid(Circle);
const std::type_info& v = typeid(c);

// type_info object could be compared directly
if (typeid(c) == typeid (b)) {
    //
}
std::cout << t.name() << std::endl;
```



## Unified Initialization

unified initialization with {}

**Direct Initialization**

```c++
std::string str{"hello"};
int i{2011};
```

**Copy Initialization**

```c++
std::string str = {"hello"};
int i = {2011};
```

The difference is that *direct initialization directly calls the constructor of the type*, whereas, in copy initialization, the value is created and implicitly converted into the type.



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