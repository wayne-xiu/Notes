# C++ Best Practice

[toc]

## 3. Use the Tools: Automated Tests

need a single command to run tests.

- [Catch2](https://github.com/catchorg/Catch2)
- [Google Test](https://github.com/google/googletest)
- [Boost.Test](https://www.boost.org/doc/libs/1_74_0/libs/test/doc/html/index.html)

[ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) is a test runner for CMake that can be used with any of the above frameworks. It is utilized with the [add_test](https://cmake.org/cmake/help/latest/command/add_test.html) feature of CMake

Automated test is critical.

- If a component is hard to test, it is not properly designed
- if a component is easy to test, it is a good indication that it is properly designed
- If a component is properly designed, it is easy to test

## 4. Use the Tools: Continuous Builds

When you start to combine multiple compilers across multiple platforms and architectures, it becomes increasingly likely that a significant change on one platform will break one or more other platforms.

To solve this problem, enable continuous builds with continuous tests for projects:

- Test all possible combinations of platforms that support
- Test Debug and Release separately
- Test all configuration options
- Test against newer compilers than you support or require

Continuous builds environment - TODO

- GitLab, GItHub actions etc.

## 5. Use the Tools: Compiler warnings

-Wall, -Wextra, -Werror

-Wpedantic (GCC/Clang) and /permissive- (MSVC)

## 6. Exercise: Use the Tools: Static Analysis

Static analysis tools are tools that analyze code without compiling or executing it.

*cppcheck* and *clang-tidy* are two popular and free tools with major IDE and editor integration

Enable

- Visual Studio: MIcrosoft's static analyzer; Clang Power Tools and cppcheck addon for VS
- CMake: Enable cppcheck and clang-tidy integration



## 7. Use the Tools: Sanitizers

Sanitizers are runtime analysis tools for C++ and are built into GCC, Clang, and MSVC

Address sanitizer, UB sanitizer, Thread sanitizer can find many issues almost like magic. always enable ASan and UBSan during development

```bash
gcc -fsanitize=address, undefined <filetocompile>
```

Use sanitizers with CMake

```cmake
# Note: by default ENABLE_DEVELOPER_MODE is True, this means that all analysis (sanitizers, static analysis) is enabled and all warnings are treated as errors
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'")
```



## 9. C++ Is Not Magic

Consider using GitHub gists as a simple way to save and share experimental tests with others - TODO

## 10. C++ Is Not Object-Oriented

C++ paradigms

- Procedural
- OOP
- Functional
- Generic
- Compile-time (`constexpr` and template metaprogramming)



## 12. `const` Everything That's Not `constexpr`

## 13. `constexpr` Everything Known at Compile Time

`constexpr` should be default instead of `#define`

```c++
// instead
static const std::vector<int> angles{-90, -45, 0};
// we should; the size is known at compile time and avoid dynamic allocation
static constexpr std::array<int, 3> angles{-90, -45, 0};
```

> `static constexpr` here is necessary to make sure the object is not reinitialized each time the function/declaration is encountered. With `static` the variable lasts for the lifetime of the program, and it will be initialized exactly once



## 14. Prefer `auto` in Many Cases

```c++
const std::string value = get_string_value();
// the return type of get_string_value could be std::string, std::string_view or const char*
// avoid conversion
const auto value = get_string_value();
```

```c++
// C++14
template<typename Numerator, typename Denominator>
auto divide(Numerator numerator, Denominator denominator) {
    return numerator / denominator;
}
```

Rules for `auto` deduction - TODO

Understand how `auto` and `template` deduction relate



## 15. Prefer ranged-for loop Syntax Over Old Loops

> note: never mutate the container itself while iterating inside of a ranged-for loop

clang-tidy's modernize-loop-convert check; `clang-tidy` is [bundled with VS code](https://devblogs.microsoft.com/cppblog/visual-studio-code-c-december-2021-update-clang-tidy/) by default now

## 16. Use `auto` in ranged for loops

- `const auto&` for non-mutating loops
- `auto&` for mutating loops
- `auto&&` only when working with weird types like `std::vector<bool>` or if moving elements out of the container - TODO 



## 17. Prefer Algorithms Over Loops

C++20 has ranges, which make algorithms more comfortable to use.

It's possible, taking a functional approach and using algorithms, that we can write C++ that reads like a sentence.

Ranges can be composed and have full support for `constexpr`



## 18. Don't Be Afraid of Templates

Templates are the ultimate DRY principle in C++. 

Don't use `T`, give type a meaningful name

## 19. Don't Copy and Paste Code

C++14 style lambdas, with generic (aka `auto`) parameters, give you a simple and easy to use method for creating reusable code that can be shared with different data types while not having to deal with `template` syntax.

copy-paste-detectors: [PMD CPD tool](https://pmd.github.io/latest/pmd_userdocs_cpd.html)



## 20. Follow the Rule of 0

No destructor is always better when its the correct thing to do. Empty destructors can destroy performance.

- make the type no longer trivial
- Implicitly disable move operations

`std::unique_ptr` can help to apply the Rule of 0 if provide a custom deleter

> Some uses of the pimpl idiom require to define a destructor. In this case, be sure to follow [Rule of 5](#21. If Must Do Manual Resource Management, Follow the Rule of 5[)

## 21. If Must Do Manual Resource Management, Follow the Rule of 5

If you provide a destructor because `std::unique_ptr` doesn't make sense for your use case, you must `=delete =default` or implement other special member functions.

```c++
struct S {
    S();
    // if you define any of the following, must deal with all the others
    S(const S&);
    S(S&&);
    S& operator=(const S&);
    S& operator=(const S&&);
};
```

We should also follow the Rule of 5 when declaring base classes with virtual functions

```c++
struct Base {
    virtual void do_stuff();
    // need virtual destructor
    virtual ~Base() = default;
    
    S(const S&) = delete;
    S(S&&) = delete;
    S& operator=(const S&) = delete;
    S& operator=(S&&) = delete;
};

struct Derived: Base {
    // we don't need to define any of the special members
    // they all inherit from 'Base'
};
```

> instead of `=delete` we can consider making special members `protected` (delete is still better)



## 22. Don't Invoke Undefined Behavior (UB)

have your enabled for UBSan, ASan, and have your warnings enabled

## 23. Never Test for `this` To Be `nullptr`, It's UB

Compilers today will always remove this check

there is no scenario in which a check `if (this)` will return false on a modern compiler

## 24. Never Test for A Reference To Be `nullptr`, It's UB

It's UB to make a null reference. Always assume a reference refers to a valid object.

## 25. Avoid `default` In `switch` Statements

why?

```c++
// NO
std::string_view get_name(Values value) {
    switch(value) {
        case Values::val1: return "val1";
        case Values::val2: return "val2";
        default: return "unknown";
    }
}
// YES
std::string_view get_name(Values value) {
    switch(value) {
        case Values::val1: return "val1";
        case Values::val2: return "val2";
    }
    return "unknown";
}
```

## 26. Prefer Scoped `enum`

`enum class` and `enum struct` are equivalent

Clang-tidy's modernizer can add `class` to your `enum` declarations

## 27. Prefer `if constexpr` over SFINAE

Before C++17, we would have used SFINAE ("Substitution Failure Is Not An Error").

```c++
#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename Numerator, typename Denominator,
		std::enable_if_t<std::is_integral_v<Numerator> &&
            			std::is_integral_v<Denominator>,
						int> = 0>
auto divide(Numerator numerator, Denominator denominator) {
	// is integer division
    if (denominator == 0) {
        throw std::runtime_error("divide by 0!");
    }
    return numerator / denominator;
}

template<typename Numerator, typename Denominator,
		std::enable_if_t<std::is_floating_point_v<Numerator> ||
            			std::is_floating_point_v<Denominator>,
						int> = 0>
auto divide(Numerator numerator, Denominator denominator) {
      // is floating point division
      return numerator / denominator;
}
```

`if constexpr` in C++17 can simplify this code

```c++
template <typename Numerator, typename Denominator>
auto divide(Numerator numerator, Denominator denominator)
{
    if constexpr (std::is_integral_v<Numerator> &&
                  std::is_integral_v<Denominator>)
    {
        if (denominator == 0)
        {
            throw std::runtime_error("divide by 0!");
        }
    }
    return numerator / denominator;
}
```

`if constexpr` is not the same as `#define`

## 28. Constrain Template Parameters with Concepts (C++20)

Concepts will result in better error message and better compile times than SFINAE. Besides much more readable code than SFINAE.

> Concepts can define complex requirements, including expected members.

TODO

## 29. De-template-ize Generic Code

Move things outside templates when we can.

De-template-ization will improve compile times and reduce binary sizes. Both are helpful.

Similar techniques apply to base classes and templated derived classes

## 30. Use Lippincott Functions

Similar to "de-template-ize" your code: this is do-not-repeat-yourself principle for exception handling routines.

A Lippincott function provides a centralized exception handling routine.

```c++
void handle_exception() {
    try {
        throw;  // re-throw exception already in flight
    } catch (const std::runtime_error& ) {}
      catch (const std::exception& ) {}
}
```

If your project uses exceptions, there’s probably some ground for simplifying and centralizing your error handling routines.

## 31. Be Afraid of Global State

Any `non-const static` value or `std::shared_ptr<>` could potentially be global state. It is never known who might update the value or if it is thread-safe to do so.

Global state can result in subtle and difficult to trace bugs where one function changes global state, and another function either relies on that change or is adversely affected by it.

> Make all `static` variables `const`; `constexpr` is even better

global singleton logger? Do the singletons have threading initialization issues (what happens if two threads try to access one of the objects for the first time at the same time?)



## 32. Make your interfaces hard to use wrong

## 33. Consider if Using the API Wrong Invokes Undefined Behavior

Some developers make the distinction between "internal" and "external" APIs. They allow unsafe APIs for internal use only.

The C++ Guideline Support Library (GSL) has a `not_null` pointer type that guarantees, because of zero cost abstractions, that the pointer passed is never `nullptr`

`std::string_view` (C++17) and `std::span` (C++20) are great alternatives to pointer/length pairs passed to functions.

## 34. Use [[nodiscard]] Liberally

`[[nodiscard]]` (C++17) is a C++ **attribute** that tells the compiler to warn if a return value is ignored.

```c++
struct [[nodiscard]] ErrorCode{};
ErrorCode get_value();
int main() {
    // warning, [[nodiscard]] value ignored
    get_value();
}
```

C++20 adds the ability to provide a description

```c++
[[nodiscard("Ignoring this result leaks resources")]]
```



## 35. Use Stronger Types

API for POSIX `socket`:

```c+
socket(int, int, int);
```

```c++
// instead of
Rectangle(int, int, int, int);
// define
struct Position {
    int x;
    int y;
};
struct Size {
    int width;
    int height;
};
struct Rectangle {
    Position position;
    Size size;
};
```

**Avoid Boolean Arguments**

`enum clss` gives you an easy way to add strong typing, avoid boolean parameters and make API harder to use wrong

```c++
// non-obvious order of parameters
struct Widget {
    // this constructor is easy to use wrong
    Widget(bool visible, boOl resizable);
}
// use below
struct Widget {
    enum struct Visible {True, False};
    enum struct Resizable {True, False};
    // better
    Widget(Visible visible, Resizable resizable);
}
```

Consider `=delete` problematic conversions

```c++
double high_precision_thing(double);
double high_precision_thing(float) = delete;
```

> any function or overload can be `=delete` in C++11



## 36. Don't return raw pointers

Returning a raw pointer makes the reader of the code and user of the library think too hard about ownership semantics. Prefer a *reference, smart pointer, non-owning pointer wrapper* or consider an optional reference.

## 37. Prefer Stack over Heap

Stack objects (locally scoped objects that are not dynamically allocated) are much more optimizer friendly, cache-friendly, and may be entirely eliminated by the optimizer. 

```c++
// ok, on stack
std::string make_string() { return "hello"; }
// bad idea, use the heap
std::unique<std::string> make_string() { return std::make_unique<std::string>("hello");}
// really bad, use heap and leaks memory
void use_string() {
    std::string* value = new std::string("hello");
}
```

The goal is no *unnecessary* heap allocations.

> Generally speaking, objects created with `new`, `make_unique`, and `make_shared` expressions are heap objects and have *Dynamic Storage Duration*. Objects created in a local scope are stack objects and have *Automatic Storage Durations*

For Java, `new` is required to create objects. don't confuse here

may run a heap profiler

## 38. No More new!

With *clang-tidy*, we can automatically convert `new` statements into `make_unique<>` and `make_shared<>` calls. Be sure to use *-fix* to apply the change after it's been discovered.



## 39. Know Your Containers

Prefer containers in this order:

- `std::array<>` : fixed-size stack-based contiguous container; size must be known at compile-time
- `std::vector<>`: dynamically-sized heap-based contiguous container

Contiguousness gives the compiler more optimization opportunities and is more *cache-friendly*

C++17's Class Template Argument Deduction

```c++
const std::array data{n+1, n+2, n+3, n+4};
// instead
const std::array<int, 4> data{n+1, n+2, n+3, n+4};
```



## 40. Avoid `std::bind` and `std::function`

possible considerable compile-time and runtime overhead.

C++14 lambdas, with generalized capture expression, are capable of the same things that `std::bind` is capable of

```c++
#include <functional>
// std::bind
auto inverted_divide = std::bind(divide, std::placeholders::_2, std::placeholders::_1);
// using lambda
auto inverted_divide = [](const auto numerator, const auto denominator) {
    return divide(denominator / numerator);
}
```

Use `Compiler Explorer` to check `compile times` and resulting `assembly`

TODO: not understanding



## 41. Skip C++11

upgrade current compiler if possible

## 42. Don't Use `initializer_list` For Non-Trivial Types

"Initializer List" is an overloaded term in C++. It is used to directly initialize values. `initializer_list` is used to pass a list of values to a function or constructor.

```c++
	vector<shared_ptr<int>> vec {
        make_shared<int>(40), make_shared<int>(2)
    };

    std::array<shared_ptr<int>, 2> arr {
        make_shared<int>(40), make_shared<int>(2)
    };
    // won't compile
    vector<unique_ptr<int>> vec_wrong{
        make_unique<int>(40), make_unique<int>(2)
    };

    cout << *(vec[0]) << ", " << *(vec[1]) << endl;
    cout << *(arr[0]) << ", " << *(arr[1]) << endl;
```

what's difference between the vector and array version??



## 43. Use the Tools: Build Generators

- CMake
- Meson
- Bazel

Raw make files or Visual Studio project files make each of the things listed above too tricky to implement. Use a build tool to help with maintaining portability across platforms and compilers.

Treat build scripts like any other code. They have their own set of best practices.

Build generators also help abstract and simplify continuous build environment with tools like `cmake --build`, which does the correct thing regardless of the platform in use.

## 44. Use the Tools: Package Managers

package managers for C++

- Vcpkg
- Conan

Package managers help with portability and reducing maintenance load on developers.

Take time to inventory project's dependencies. Compare your dependencies with what is available with the package managers above. Does any one package manager have all of the dependencies? How out of date your current packages?

> Q: Does Vcpkg from Microsoft works on Ubuntu?

## 45. Improving Build Time

a few practical considerations:

- De-template-ize your code where possible
- Use forward declarations where it makes sense to
- Enable PCH (precompiled headers) in build system
- Be ware of unity builds [CMake unity build](https://cmake.org/cmake/help/latest/prop_tgt/UNITY_BUILD.html)
- Use a build analysis tool to see where build time is spent [Clang Build Analyzer](https://github.com/aras-p/ClangBuildAnalyzer)

## 46. Use the Tools: Multiple Compilers

Support *at least 2* compilers on your platform

- Use Visual studio, switch between clang and cl.exe; can also use WSL and enable remote Linux Builds
- Use Linux, switch between GCC and Clang easily

Exercises

- Add another compiler
- Add another operating system



## 47. Fuzzing and Mutating

Your imagination limits the tests that you can create. Generating all possible inputs to all possible function calls in all possible combinations is impossible. There are tools exist to solve this problem.

**Fuzzing**

Fuzz testers generate strings of random data of various lengths. The fuzz tester analyzes coverage data generated from your test's execution and uses that information to remove redundant tests and generate new novel and unique tests.

> Fuzz testings primarily finds memory and security flaws

LLVM's libFuzzer

**Mutating**

Mutation testing works by modifying conditional and constants in the code being tested.

Any test that continues to pass is a “mutant that has survived” and may indicate either a flawed test or a bug in the code.

## 48. Continue Your C++ Education

- know how to ask questions
- Conference and local user groups



## 50. Bonus: Understand The Lambda

