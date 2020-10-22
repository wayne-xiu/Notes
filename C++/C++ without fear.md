# C++ without fear

[toc]

Type: Beginner

bullet-proofing

## Making Decisions and Loops

Fibonacci number and golden ratio

## Understanding Arrays and Vectors

```c++
int arr[5] = {10, 25, 40, 46, 90};
int arr[] = {10, 25, 40, 46, 90};
```

classic "for" and "range-for"

vector; dynamic size array

sorting the vector and calculate the median value (without checking odd or even)

## Using C++ functions

software reuse; subroutine

prototype functions for declaration

## Manipulating Strings

C-string vs. string class

```c++
getline(cin, string_name);  // gets all inputs, including space
```

stoi, stof to get numerical values from a string

## Pointers and References

- dynamic memory allocation of memory
- efficient processing of arrays
- "return" more than one value, letting a function to make permanent change to original values
- build in-memory networks (linked list, trees)

the swap function using pointer and reference respectively

```c++
// dynamically allocate an array
double* p = new double[n];
```

## Simple Classes

OOP

class and struct; classes and objects;

access control: public, protected, private

constructors and function overloading

always write a default constructor

## Trees and Nodes

new/delete and -> operators

```c++
class Lnode {
  public:
    string value;
    Lnode* next;
};
```

```c++
class BNode {
  public:
	string value;
    BNode* pLeft;
    BNode* pRight;
    BNode(string s) { value = s; pLeft = pRight = nullptr;}
}
```

write a binary tree

```c++
class BTree {
  public:
    void insert(stirng s);
    void print();
    Btree() { root = nullptr;}
  private:
    BNode* root;
}
```

```c++
class BNode {
  public:
    string val;
    BNode* pLeft;
    BNode* pRight;

    BNode (string s) {
        val = s;
        pLeft = nullptr;
        pRight = nullptr;
    }
};

class BTree {
  public:
    BTree() {
        root = nullptr;
    }
    void print() {
        print_subtree(root);
    }
    void insert(string s) {
        root = insert_at_subtree(root, s);
    }

  private:
    BNode* root;
    void print_subtree(BNode* p);
    BNode* insert_at_subtree(BNode* p, string s);
};


void BTree::print_subtree(BNode* p) {
    if (!p)
        return;
    print_subtree(p->pLeft);
    cout << p->val << endl;
    print_subtree(p->pRight);
}

BNode* BTree::insert_at_subtree(BNode* p, string s) {
    if (!p)
        return new BNode(s);
    if (s < p->val)
        p->pLeft = insert_at_subtree(p->pLeft, s);
    else if ( s > p->val)
        p->pRight = insert_at_subtree(p->pRight, s);
    return p;
}


int main() {

    BTree my_tree;
    string s;
    while(true) {
        cout << "Enter a name: ";
        getline(cin, s);
        if (s == "")
            break;
        my_tree.insert(s);
    }
    my_tree.print();

    return 0;
}
```

Search speed for insertion is fast (assuming tree is balanced). Search goes log(N), not N

The class if bullet-proof. Access to the nodes can only through root, which is private



