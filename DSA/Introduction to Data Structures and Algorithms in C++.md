# Introduction to Data Structures and Algorithms in C++ - Pluralsight

[toc]

Standard library vs. custom implementations for production code; implement from scratch for practice

CppCon2016: High performance code 201: Hybrid data structures

Prerequisite: dynamic memory allocation; stack vs. heap; constructors, destructors;

## Safely Using Arrays

safe array memory management; boundary checks;

using arrays as building blocks for complex data structures;

contiguous memory locations - cache friendly; Homogeneous data types; fast access by index O(1)

Built-in array allocations

- stack: fast allocations, limited space
- heap: slower allocations, much more room (allocate large data)

release memory



Implement an int array

```c++
class IntArray {
  private:
    int* m_ptr{nullptr};  // direct member initialization
    int m_size{0};
    bool isValidIndex(int index) const {
        return (index >= 0) && (index < m_size);
    }
  public:
    IntArray() = default;  // default constructor
    explicit IntArray(int size) {
        if (size != 0) {
            m_ptr = new int[size] {};  // {} zero-init
            m_size = size;
        }
    }

    ~IntArray() {
        if (!this->isEmpty()) {
            delete[] m_ptr;
        }
    }

    int size() const {
        return m_size;
    }
    bool isEmpty() const {
        return m_size == 0;
    }
    int operator[] (int index) const {
        if (!isValidIndex(index)) {  // index out of range
            // throw an exception
            throw std::out_of_range("The index is out of range");
        }
        return m_ptr[index];
    }
    int& operator[] (int index) {
        if (!isValidIndex(index)) {  // index out of range
            // throw an exception
            throw std::out_of_range("The index is out of range");
        }
        return m_ptr[index];
    }
};
```

User Address Sanitizer (ASan) for memory error detection, e.g. memory leak

write code defensively with exception handling

**Summary**

- safe memory management with constructor/destructor
- Safe element access with operator[]

## Improvement Array Implementation

- cout << myArray
- Copying
  - shallow vs. deep copy
  - copy-and-swap idiom
- Move semantics
- Generic Array<T> using templates

### overload <<

```c++
    friend ostream& operator<< (ostream& os, const IntArray& a) {
        os << "[ ";
        for (auto i = 0; i < a.size(); ++i)
            os << a[i] << " ";
        os << "]";
        return os;
    }
```

### Copy constructor (deep)

The above code has one bug when copy constructed/initialized; and the program would crash due to repeated destruction

```c++
    IntArray a{3};
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    IntArray b = a;  // copy bug; shallow default copy
    cout << a << endl;
    cout << b << endl;

    b[1] = 100;
    cout << a << endl;
    cout << b << endl;
```

We need explicitly define copy constructor (deep copy)

```c++
    IntArray(const IntArray& other) {
        if (!other.isEmpty()) {
            m_size = other.size();
            m_ptr = new int[m_size] {};
            for (int i = 0; i < m_size; ++i)
                m_ptr[i] = other.m_ptr[i];
        }
    }
```

### Overloading assignment operator

compiler default is member-wise copy (shallow)

**copy-and-swap idiom**

```c++
    friend void swap(IntArray& a, IntArray& b) noexcept {
        using std::swap;
        swap(a.m_ptr, b.m_ptr);
        swap(a.m_size, b.m_size);
    }

    IntArray& operator= (IntArray other) {
        // prevent self-copy x = x
        /*if (&other != this) {
            // release previous array block
            m_size = other.size();
            for (int i = 0; i < m_size; ++i)
                m_ptr[i] = other.m_ptr[i];
        }*/
        swap(*this, other);
        return *this;
    }
```

### Optimizing the Array class with move semantics

```c++
    IntArray(IntArray&& source) {
        // transfer ownership
        m_ptr = source.m_ptr;
        m_size = source.m_size;
        // clear source
        source.m_ptr = nullptr;
        source.m_size = 0;
    }
```

### Generalizing the Array class with templates

```c++
template <typename T>
class Array {
  private:
    T* m_ptr{nullptr};
    int m_size{0};

    bool isValidIndex(int index) const {
        return (index >=0) && (index < m_size);
    }
  public:
    explicit Array(int size) {
        assert(size >= 0);
        if (size != 0) {
            m_ptr = new T[size] {};
            m_size = size;
        }
    }

    Array(const Array& source) {
        if (!source.isEmpty()) {
            m_size = source.m_size;
            m_ptr = new T[m_size] {};
            for (int i = 0; i < m_size; ++i)
                m_ptr[i] = source.m_ptr[i];
        }
    }

    bool isEmpty() const {
        return m_size == 0;
    }

    T operator[] (int index) const {
        if (!isValidIndex(index))
            throw std::out_of_range("The index is out of range");
        return m_ptr[index];
    }
    T& operator[] (int index) {
        if (!isValidIndex(index))
            throw std::out_of_range("The index is out of range");
        return m_ptr[index];
    }

    int size() const {
        return m_size;
    }

    friend ostream& operator<< (ostream& os, const Array& a) {
        os << "[ ";
        for (auto i = 0; i < a.size(); ++i)
            os << a[i] << " ";
        os << "]";
        return os;
    }
};
```

## Efficiently Searching

compile time const - constexpr

linear and binary search

asymptotic runtime complexity

## Implementing a Last-in First-out Pattern with Stack

The nested function calls follows a LIFO pattern

**fundamental stack operations**

- push(const T& elem)
- pop()
- top()

```c++
template <typename T>
class Stack {
  private:
    Array<T> m_array;
    int m_top;
  public:
    explicit Stack(int maxStackSize): m_array{maxStackSize}, m_top{-1} {}
    void push(const T& element) {
        m_top++;
        m_array[m_top] = element;
    }
    T pop() {
        T topElement = m_array[m_top];
        m_top--;
        return topElement;
    }
    const T& top() const {
        return m_array[m_top];
    }

    int size() const {
        return m_top+1;
    }
    bool isEmpty() const {
        return size() == 0;
    }
    int maxSize() const {
        return m_array.size();
    }
    void clear() {
        m_top = -1;
    }

    friend std::ostream& operator<< (std::ostream& os, const Stack<T>& stack) {
        if (stack.isEmpty()) {
            os << "[*** Empty Stack ***]\n\n";
            return os;
        }
        os << "[Stack]\n";
        for (int i = stack.m_top; i >= 0; i--)
            os << " " << stack.m_array[i] << "\n";
        os << "\n";

        return os;
    }
};
```

Potential issue: **stack overflow**

```c++
    void push(const T& element) {
        if (size() >= maxSize()) {
            throw StackOverFlowExceptionP{};  // could inherit from runtime exception
        }
        m_top++;
        m_array[m_top] = element;
    }
```

## Node-based Data Structure: Linked List

a chain of nodes; singly linked list; double linked list

insertion; remove (pay attention to memory release); 

```c++
#ifndef LIST_HPP_INCLUDED
#define LIST_HPP_INCLUDED

#include <cassert>    // For assert()
#include <ostream>    // For printing to ostream

// A singly linked list
template <typename T>
class List {
 private:
  struct Node {
    Node* Next{nullptr};
    T     Data{};

    // Create a default empty node
    Node() = default;

    // Create a node storing input data
    explicit Node(const T& data)
      : Data{data} {}

    // Create a node storing input data, and pointing to another node
    Node(const T& data, Node* next)
      : Data{data}, Next{next} {}
  };   
  
  Node* m_head{nullptr};
  int   m_count{0};

  // Ban copy
  List(const List&) = delete;
  List& operator=(const List&) = delete;


 public:

  typedef Node* Position;

  List() = default;

  ~List() {
    Clear();
  }

  int Count() const {
    return m_count;
  }  

  bool IsEmpty() const {
    return (m_count == 0);
  }

  void Clear() {
    while (! IsEmpty()) {
      RemoveHead();
    }
  }

  void InsertHead(const T& value) {
    Node* node = new Node{value};

    // Replace current head with new node
    node->Next = m_head;
    m_head = node;

    ++m_count;
  }

  void RemoveHead() {
    // It's invalid to attempt to remove head from an empty list
    assert(! IsEmpty());

    // Save pointer to head's next node, before removing current head;
    // basically, head's next node will become the *new* head
    Node* newHead = m_head->Next;
    
    // Remove current head
    delete m_head;

    // Update head, pointing to the node that followed the old head
    m_head = newHead;

    --m_count;
  }

  T ElementAt(Position node) {
    assert(! IsEmpty());
    assert(node != nullptr);    

    return node->Data;
  }

  void InsertAfter(Position node, const T& value) {
    assert(node != nullptr);

    Node* newNode = new Node{value};
    
    // This new node is inserted between 'node' and 'node->Next'
    newNode->Next = node->Next;
    node->Next = newNode;

    ++m_count;
  }

  void RemoveAfter(Position node) {
    assert(! IsEmpty());
    assert(node != nullptr);

    Node* obsoleteNode = node->Next;
    node->Next = obsoleteNode->Next;

    delete obsoleteNode;

    --m_count;
  }

  Position Find(const T& value) {
    if (IsEmpty()) {
      return nullptr;
    }

    // Linear search
    Node* node = m_head;
    while (node != nullptr) {
      // Compare current node's data with the search value
      if (node->Data == value) {
        // Found!
        return node;
      }

      // Move forward to the next node in the list
      node = node->Next;
    }

    // After traversing the whole list, no matching element was found
    return nullptr;
  }

  friend std::ostream& operator<<(std::ostream& os, const List<T>& list) {
    if (list.IsEmpty()) {
      os << "[ Empty List ]";
      return os;
    }
    
    os << "[ ";

    // For each node:
    Node* node = list.m_head;
    while (node != nullptr) { 
      // Print data stored in current node
      os << node->Data << ' ';

      // Move forward to the next node
      node = node->Next;
    }

    os << ']';
    return os;
  } 

};

#endif // LIST_HPP_INCLUDED
```

```c++
#include "list.h"
#include <iostream>
using std::cout;

int main() {
    List<int> l{};
    cout << " Created an empty list: " << l << "\n\n";

    cout << " Inserting some elements...\n";
    l.InsertHead(10);
    l.InsertHead(64);
    l.InsertHead(80);
    l.InsertHead(77);
    cout << " Current list: " << l << "\n\n";

    cout << " Inserting a new element (500) after node with value 64.\n";
    auto pos = l.Find(64);
    l.InsertAfter(pos, 500);
    cout << " Current list: " << l << "\n\n";

    cout << " Removing current head.\n";
    l.RemoveHead();
    cout << " Current list: " << l << "\n\n";

    cout << " Clearing the whole list.\n";
    l.Clear();
    cout << " Current list: " << l << "\n\n";
}
```

