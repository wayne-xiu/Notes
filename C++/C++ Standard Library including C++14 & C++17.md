# C++ Standard Library including C++14 & C++17

[toc]

Components of STL

![STLComponent](Media/STLComponent.png)

**Containers**

- Sequential containers: vector, std::array, deque, list, forward_list (vector should be default choice for 95% cases)
- Associative containers: ordered or unordered: set, map, multiset, multimap; unordered_set, unordered_map, unordered_multiset, unordered_multimap (map should be default choice for 95% cases)
- Container Adapters: provide a simplified interface to the sequential containers: stack, queue and priority_queue

> The ordered associative containers have access time depending logarithmically on the number of elements, the unordered associative containers allow constant access time. Therefore the access time is independent of the size.

**Algorithms**

STL provides more than 100 algorithms. By specifying the execution policy, we can run most of the algorithms *sequential, parallel or parallel and vectorized*. A lot of algorithms can be further customized by *callable* like functions, function objects or lambda-functions.

Text Processing

- String
- String View: std::string_view is quite cheap to copy and is a non-owning reference to a std::string
- Regular Expression

Multithreading 

- Threads
- Shared variables: coordinated with mutexes or locks
- Thread-local variables
- Condition variables: sender-receiver workflow
- Tasks: similar to threads. While a programmer explicitly creates a thread, a task will be implicitly created by the C++ runtime. Tasks are like data channels. The promise puts data into the data channel; the future picks the value up.

> All needed headers should always be explicitly specified, even though the compiler may add additional headers to the header files.

nested namespaces

```c++
auto timeNow = std::chrono::system_clock::now();
// or 
using std::chrono::system_clock;
auto timeNow = now();
```

> using directives should be used with great care in source files. That includes names, which accidently hide names in the local or surrounding namespace.
>
> Don't use using directive in header files.

```c++
// Namespace alias
#include <chrono>
namespace systemClock = std::chrono::system_clock;
```

## Useful Functions

- min, max, minmax; By default, the less operator (<) is used for comparison, and can be customized. Functions that return true or false are called predicates
- std::move: the compiler converts the source arg to a rvalue reference

> Move is cheaper than copy: 1. no superfluous allocation and deallocation of memory necessary 2. there are objects that can not be copied, e.g. a thread or lock

- std::forward: empowers writing function templates, which can identically forward their arguments. Typical use cases for std::forward are factory functions or constructors. Factory functions are functions which create an object and must therefore identically pass the arguments. Constructors often use their arguments to initialize their base class with the identical arguments.

```c++
#include <utility>

using std::initializer_list;
struct MyData {
    MyData(int, double, char) {}
};

template<typename T, typename... Args>
T createT(Args&&... args) {  // universal reference
    return T(std::forward<Args>(args)...);
}

int main() {
    int a = createT<int>();
    int b = createT<int>(1);

    string s = createT<string>("Only for testing.");
    MyData myData2 = createT<MyData>(1, 3.19, 'a');

    typedef vector<int> IntVec;
    IntVec intVec = createT<IntVec>(initializer_list<int>({1, 2, 3}));

    cout << a << " " << b << endl;
    for (auto item: intVec)
        cout << item << " ";
    cout << endl;

    return 0;
}

```

A universal reference (or forwarding reference) is an rvalue reference in a type deduction context

> std::forward in combination with variadic templates allows completely generic functions, which can accept an arbitrary number of arguments and forward them unchanged.

