# C++ 实战笔记

[toc]

## 1. 课前导读

重点是谈使用时的注意事项和经验教训。

课程分为5个模块，注重语言和库的“开发落地”，基本不讲语法细节和内部实现原理，用实例促使更多地应用“现代C++”自然、直观的思维方式。

- 概论：生命周期和编程范式
- 语言特性：自动类型推导、智能指针、Lamdba表达式
- 标准库：字符串、容器、算法和并发
- 技能进阶：第三方工具，序列化、网络通信和性能分析
- 总结：设计模式，完整可用的C++服务端程序

勉言：

- 任何人都能写出机器能看懂的代码，但只有优秀的程序员才能写出人能看懂的代码。
- 有两种写程序的方式：一种是把代码写得非常复杂，以至于“看不出明显的错误”；另一种是把代码写得非常简单，以至于“明显看不出错误”。
- “把正确的代码改快速”，要比“把快速的代码改正确”，容易得太多。

C++/Python混合编程，C++写底层代码，Python调用。



建议使用Linux。目前差不多所有的商业网站的服务器（还有Android）上跑的都是Linux，C++再开发后台服务方面可以大显身手。

居然不会CMake？

## 2. 概论

从“生命周期”和“编程范式”的角度来“剖析”C++

### 重新认识C++：生命周期和编程范式 

Coding, Pre-processing, Compiling and Running

预处理是C/C++程序独有的阶段 #

“模板元编程”



现代C++支持“面向过程”、“面向对象”、“范式”、“模板元”、“函数式”这五种编程范式。

![C++ProgrammingParadigm](../Media/C++ProgrammingParadigm.png)

面向对象：高内聚低耦合，核心思想是“抽象”和“封装”

泛型编程：核心思想是“一切皆为类型”，使用模板而不是继承的方式来复用代码，运行效率更高，代码也更简洁；STL

模板元编程：核心思想是“类型运算”，操作的数据是编译时可见的“类型”，代码只能由编译器执行，而不能被运行时的CPU执行。更多的以库的方式来使用

函数式：数学意义上、无副作用的函数，核心思想时“一切皆可调用”，通过一系列连续或这嵌套的函数调用实现对数据的处理。

适当混用：常用范式“过程+对象+泛型”，少量“函数式”，慎用“模板元”

C++不是纯粹的面向对象语言，它是面向对象的探路先锋，走了一些弯路，给后来的Java等蹚了道；我们完全可以在C++里按照Java的风格来写面向对象。



面向对象和基于对象有什么区别？

广义上都属于面向对象。前者强调统一用对象建模，多应用设计模式，对象关系复杂。后者相当于C with class，只把class当成struct来封装数据，继承、多态等高级特性用的比较少。



C++的应用水平取决于团体的整体水平。

typedef是在编译阶段，而不是预处理阶段

编译成机器码，但这并不是跨平台的代码，而是源码可以跨平台，由不同的编译器编译成对应平台的二进制代码。
Java等语言的字节码由于运行在虚拟机上，它才是跨平台的代码。

docker用的是go。C++可以让你理解底层计算机的运行机制，反而让你在上层更好地工作。而且C++很容易编写各种底层模块给上层调用，做混合编程里最核心最要求性能的那部分。

### 编码阶段能做什么：秀出好的code style

human readable

空格与空行：留白的艺术

起个好名字

- “匈牙利命名法” - 基本被淘汰
- “CamelCase" - popular in Java
- "snake_case" - C/C++

命名长度一个普遍认可的原则：变量/函数的名字长度与它的作用域成正比；局部可以短一点，全局名应该长一点。

注释：适当注释，一个突出的反例十四JSON，没有注释功能。参考Javadoc

Doxgen：支持从源码生成完整的API文档，但是格式过于“死板”。

注释另一个功能是TODO

“必杀技”：code review

检查C++代码风格的工具：cpplint。它是一个Python脚本

clang-format

```bash
sudo pip install cpplint
```

函数的注释如果是API，写在声明，如果是内部用，写在定义处。



### 预处理阶段能做什么：宏定义和条件编译      

#### 预处理编程：#include, #define

记住：预处理阶段编程的操作目标是“源码”，用各种指令控制预处理器，把源码改造成另一种形式。虽然都在一个源文件里，但预处理不属于C++语言，它走的是预处理器，不受C++语法规则的约束。

#include可以不做检查的包含任意的文件（不限于“头文件”）

#include可以编写一些代码片段，存进“.inc”文件里，然后有选择的加载。用得好的话，可以实现“源码级别的抽象”。

```c++
static uint32_t calc_table[] = { // 非常大的一个数组，有几十行
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    ...
};

static uint32_t calc_table[] = {
#	include "calc_values.inc" // 非常大的一个数组，细节被隐藏
};
```



#define可谓“无所不能”，在预处理阶段可以无视C++语法限制，替换任何文字，定义常量/变量,实现函数功能，为类型起别名（typedef），减少重复代码...。但是，使用宏的时候一定要谨慎，时刻记着以简化代码、清晰易懂为目标，不要“滥用”。

注意事项：

- 首先，因为宏的展开、替换发生在预处理阶段，不涉及函数调用、参数传递、指针寻址，没
  有任何运行期的效率损失，所以对于一些调用频繁的小代码片段来说，用宏来封装的效果比
  inline 关键字要更好，因为它真的是源码级别的无条件内联。

  ```c++
  // 示例来自Nginx
  #define ngx_tolower(c) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
  #define ngx_toupper(c) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
  #define ngx_memzero(buf, n) (void) memset(buf, 0, n)
  ```

- 其次，你要知道，**宏是没有作用域概念的，永远是全局生效**。所以，对于一些用来简化代
  码、起临时作用的宏，最好是用完后尽快用“#undef”取消定义，避免冲突的风险

  ```c++
  #define CUBE(a) (a) * (a) * (a) // 定义一个简单的求立方的宏
  cout << CUBE(10) << endl; // 使用宏简化代码
  cout << CUBE(15) << endl; // 使用宏简化代码
  #undef CUBE // 使用完毕后立即取消定义
  ```

  另一种做法是宏定义前先检查，如果之前有定义就先undef，然后再重新定义

  ```c++
  #ifdef AUTH_PWD // 检查是否已经有宏定义
  # undef AUTH_PWD // 取消宏定义
  #endif // 宏定义检查结束
  #define AUTH_PWD "xxx" // 重新宏定义
  ```

- 再次，你可以适当使用宏来定义代码中的常量，消除“魔术数字”“魔术字符串”（magic number）。虽然不少人认为，定义常量更应该使用 enum 或者 const，但我觉得宏定义毕竟用法简单，也是源码级的真正常量，而且还是从 C 继承下来的传统，用在头文件里还是有些优势的。

  ```c++
  #define MAX_BUF_LEN 65535
  #define VERSION "1.0.18"
  ```

- “文本替换”的功能

  ```c++
  #define BEGIN_NAMESPACE(x) namespace x {
  #define END_NAMESPACE(x) }
  BEGIN_NAMESPACE(my_own)
  ... // functions and classes
  END_NAMESPACE(my_own)
  ```



#### 条件编译（#if/#else/#endif）

通常编译环境都会有一些预定义宏，比如 CPU 支持的特殊指令集、操作系统 / 编译器 / 程序库的版本、语言特性等，使用它们就可以早于运行阶段，提前在预处理阶段做出各种优化，产生出最适合当前系统的源码。

```c++
// __cplusplus 标记了C++的版本号
#ifdef __cplusplus // 定义了这个宏就是在用C++编译
	extern "C" { // 函数按照C的方式去处理
#endif
	void a_c_function(int a);
#ifdef __cplusplus // 检查是否是C++编译
	} // extern "C" 结束
#endif

#if __cplusplus >= 201402 // 检查C++标准的版本号
	cout << "c++14 or later" << endl; // 201402就是C++14
#elif __cplusplus >= 201103 // 检查C++标准的版本号
	cout << "c++11 or later" << endl; // 201103是C++11
#else // __cplusplus < 201103 // 199711是C++98
	# error "c++ is too old" // 太低则预处理报错
#endif // __cplusplus >= 201402 // 预处理语句结束
```



使用“#if 1”“#if 0”来显式启用或者禁用大段代码，要比“/* … */”的注释方式安全得多，也清楚得多

```c++
#if 0 // 0即禁用下面的代码，1则是启用
	... // 任意的代码
#endif // 预处理结束
#if 1 // 1启用代码，用来强调下面代码的必要性
	... // 任意的代码
#endif // 预处理结束
```



小结：

1. 预处理不属于 C++ 语言，过多的预处理语句会扰乱正常的代码，除非必要，应当少用慎
   用；
2. “#include”可以包含任意文件，所以可以写一些小的代码片段，再引进程序里；
3. 头文件应该加上“Include Guard”，防止重复包含；
4. “#define”用于宏定义，非常灵活，但滥用文本替换可能会降低代码的可读性；
5. “条件编译”其实就是预处理编程里的分支语句，可以改变源码的形态，针对系统生成最合适的代码。

> 有的编译器支持指令"#pragma once"，也可以实现“include guard”，但它是非标准的，不推荐使用 - 这个g++, MSVC都支持
>
> C++20新增了“模块”(module)特性，可以实现一次性加载，但"include guard"再短期内还是无可替代的。

条件编译多用在跨平台和高级优化方面，不用当然也可以，但要追求性能极致就必须考虑。

预处理就是文本替换，你要是愿意，用shell、Python也可以处理C++源码来实现。

extern "C" { }是处理编译器的链接符号，保持与C兼容，通常是为了导出外部接口使用的。

预处理就是面向源码编程，调整源码的形态，掌握了这一点就可以让代码更干净整齐。



### 编译阶段能做什么：属性和静态断言

编译阶段生成machine instruction code

编译阶段的特殊性在于，它看到的都是 C++ 语法实体，比如 typedef、using、template、struct/class 这些关键字定义的类型，而不是运行阶段的变量。所以，这时的编程思维方式与平常大不相同。

```c++
// 编译阶段数值计算
template <int N>
struct fib
{
    static const int value = fib<N - 1>::value + fib<N - 2>::value;
};

template <>
struct fib<0> // template specialization
{
    static const int value = 1;
};

template <>
struct fib<1>
{
    static const int value = 1;
};

int main()
{
    cout << fib<2>::value << endl;
    cout << fib<3>::value << endl;
    cout << fib<4>::value << endl;
    cout << fib<5>::value << endl;

    return 0;
}
```

上面的计算是编译阶段完成的！

可以把编译器想象成是一种特殊的“虚拟机”，在上面跑的是只有编译器才能识别、处理的代码。



两个比较容易理解的编译阶段技巧：属性和静态断言

#### 属性（attribute）

C++11, attribute 可以被理解为给变量、函数、类等“贴”上一个编译阶段的“标签”，方便编译器识别处理。

```c++
[[deprecated("deadline:2020-12-31")]] // C++14 or later
int old_func();
```

任何用到这个函数的程序都会在编译时看到这个标签，报出一条警告。当然，程序还是能够正常编译的

#### 静态断言（static _assert）

```c++
assert(i > 0 && "i must be greater than zero");
assert(p != nullptr);
assert(!str.empty());
```

注意，assert 虽然是一个宏，但在预处理阶段不生效，而是在运行阶段才起作用，所以又叫“动态断言”。

static_assert是编译阶段里检测各种条件的“断言”，编译器看到static_assert会计算表达式的值，如果false，会报错，导致编译失败。

```c++
template<int N>
struct fib
{
    static_assert(N >= 0, "N >= 0");
    static const int value =
    	fib<N - 1>::value + fib<N - 2>::value;
};
```

> assert是宏，但是static_assert是专门的关键字，不是宏
>
> static_assert 运行在编译阶段，只能看到编译时的常数和类型，看不到运行时的变量、指针、内存数据等，是“静态”的，所以不要简单地把 assert 的习惯搬过来用。

想要更好地发挥静态断言的为例，还要配合标准库里的"type_traits", 它提供了对应这些概念的各种编译期“函数”

```c++
// 假设T是一个模板参数，即template<typename T>
static_assert(
	is_integral<T>::value, "int");
static_assert(
	is_pointer<T>::value, "ptr");
static_assert(
	is_default_constructible<T>::value, "constructible");
```



小结：

1. “属性”相当于编译阶段的“标签”，用来标记变量、函数或者类，让编译器发出或者不发出警告，还能够手工指定代码的优化方式。
2. 官方属性很少，常用的只有“deprecated”。我们也可以使用非官方的属性，需要加上名字空间限定。
3. static_assert 是“静态断言”，在编译阶段计算常数和类型，如果断言失败就会导致编译错误。它也是迈向模板元编程的第一步。
4. 和运行阶段的“动态断言”一样，static_assert 可以在编译阶段定义各种前置条件，充分利用 C++ 静态类型语言的优势，让编译器执行各种检查，避免把隐患带到运行阶段。



### 面向对象编程：怎样才能写出一个“好”的类？      

“面向对象”仍然是编程界的主流 C++, Java, Python, Go, Rust, Swift...

#### 设计思想

要点是抽象和封装

#### 实现原则

“继承”和“多态”不是核心，只能算是附加品。建议再设计类的时候*尽量少用继承和虚函数* - （不敢苟同，继承要少用，多态是很有益处的）。

如果没有隐含的重用代码也会降低耦合度，让类更独立，更容易理解。
还有，把“继承”切割出去之后，可以避免去记忆、实施那一大堆难懂的相关规则，比如public/protected/private 继承方式的区别、多重继承、纯虚接口类、虚析构函数，还可以绕过动态转型、对象切片、函数重载等很多危险的陷阱，减少冗余代码，提高代码的健壮性。

如果非要用继承不可，那么我觉得一定要控制继承的层次，用 UML 画个类体系的示意图来辅助检查。如果继承深度超过三层，就说明有点“过度设计”了，需要考虑用组合关系替代继承关系，或者改用模板和泛型。

设计模式、重构

少用“嵌套类”，这本来是名字空间该做的事情，用类来实现就有点“越权”了。正确的做法应该是，定义一个新的名字空间，把内部类都“提”到外面，降低原来类的耦合度和复杂度。

#### 编码准则

use "final"

建议你只使用 public 继承，避免使用 virtual、protected

在现代 C++ 里，一个类总是会有六大基本函数：三个构造、两个赋值、一个析构。

=default

=delete

explicit: 因为 C++ 有隐式构造和隐式转型的规则，如果你的类里有单参数的构造函数，或者是转型操作符函数，为了防止意外的类型转换，保证安全，就要使用“explicit”将这些函数标记为“显式”。

#### 常用技巧

##### “委托构造” delegating constructor

传统的init()函数可行，但效率和可读性较差，毕竟不是真正的构造函数。C++11引入了“委托构造”的新特性

```c++
class DemoDelegating final
{
private:
	int a; // 成员变量
public:
    DemoDelegating(int x) : a(x) {}// 基本的构造函数

    DemoDelegating() : // 无参数的构造函数
    	DemoDelegating(0) {}// 给出默认值，委托给第一个构造函数

    DemoDelegating(const string& s) : // 字符串参数构造函数
    	DemoDelegating(stoi(s)) {} // 转换成整数，再委托给第一个构造函数
};
```

##### 成员变量初始化 in-class member initializer

```c++
class DemoInit final // 有很多成员变量的类
{
private:
    int a = 0; // 整数成员，赋值初始化
    string s = "hello"; // 字符串成员，赋值初始化
    vector<int> v{1, 2, 3}; // 容器成员，使用花括号的初始化列表
public:
    DemoInit() = default; // 默认构造函数
    ~DemoInit() = default; // 默认析构函数
public:
    DemoInit(int x) : a(x) {} // 可以单独初始化成员，其他用默认值
};
```

##### 类型别名 Type alias

C++11拓展了using的用法，增加了typedef的功能（格式相反）

```c++
using uint_t = unsigned int; // using别名
typedef unsigned int uint_t； // 等价的typedef
```

```c++
class DemoClass final
{
public:
    using this_type = DemoClass; // 给自己也起个别名
    using kafka_conf_type = KafkaConfig; // 外部类起别名
public:
    using string_type = std::string; // 字符串类型别名
    using uint32_type = uint32_t; // 整数类型别名
    using set_type = std::set<int>; // 集合类型别名
    using vector_type = std::vector<std::string>;// 容器类型别名
private:
    string_type m_name = "tom"; // 使用类型别名声明变量
    uint32_type m_age = 23; // 使用类型别名声明变量
    set_type m_books; // 使用类型别名声明变量
private:
    kafka_conf_type m_conf; // 使用类型别名声明变量
};
```

类型别名让代码规范整齐，而且因为引入了“语法层面的宏定义”，将来在维护时可以随意改换成其他类型。。比如，把字符串改成 string_view（C++17 里的字符串只读视图），把集合类型改成 unordered_set，只要变动别名定义就行了，原代码不需要做任何改动。



小结：

1. “面向对象编程”是一种设计思想，要点是“抽象”和“封装”，“继承”“多态”是衍生出的特性，不完全符合现实世界。
2. 在 C++ 里应当少用继承和虚函数，降低对象的成本，绕过那些难懂易错的陷阱。
3. 使用特殊标识符“final”可以禁止类被继承，简化类的层次关系。类有六大基本函数，对于重要的构造 / 析构函数，可以使用“= default”来显式要求编译器使用默认实现。
4. “委托构造”和“成员变量初始化”特性可以让创建对象的工作更加轻松。
5. 使用 using 或 typedef 可以为类型起别名，既能够简化代码，还能够适应将来的变化。

在C++里，不要再使用"typedef struct {...} xxx;" 的方式来定义结构体，这是传统C的做法，不仅没有必要，而且会造成困惑。

传统的类编写方式时“\*.h + \*.cpp”， 声明与实现分离，但更推荐在一个“\*.hpp”里实现类的全部功能，这样更“现代”。很多开源的现代C++项目都采用了“hpp”的方式，比如Boost。在.h中将类的定义和实现写在一起，是默认所有成员函数都内联了，但是否内联有编译器决定，通常只有小的函数才会内联，大的函数不会。

设计模式更强调对象组合，而不是继承；继承时一种“硬”代码复用，关系比较强，不如组合灵活，建议少用。

## 3. 语言特性

## 4. 标准库

## 5. 技能进阶

## 6. 总结篇

## 7. 结束语

## 8. 轻松话题