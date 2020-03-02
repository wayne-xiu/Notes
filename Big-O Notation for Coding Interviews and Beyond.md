# Big-O Notation for Coding Interviews and Beyond

[toc]

## Basics

Integer array sorting with Bubble sort, Merge sort and Dual Pivot Quicksort (Java's util.Arrays.sort).

Generally speaking, time and space have an inversely proportionally relationship with each other; if space increases then execution time decreases, and if execution time increases then space decreases.

## Formal Analysis Tools

**Big $\Theta$**

$f(n)$ belongs to the set $\Theta(g(n))$

​																	$$0\le c_1g(n) \le f(n) \le c_2g(n)$$

​																		$$f(n)=\Theta g(n)$$

**Big O**

​																	$$0 \le f(n) \le cg(n)$$

**Big $\Omega$**

​																	$$0 \le g(n) \le f(n)$$

- Big $\Theta$ provides both an asymptotically upper and lower tight bound
- Big O only provides an asymptotic upper bound which may not necessarily be a tight bound
- Big $\Omega$ provide an asymptotic lower bound which may not necessarily be a tight bound
- if $f(n)=\Theta(g(n))$ then it implies that $f(n)=O(g(n))$
- if $f(n)=\Theta(g(n))$ then it implies that $f(n)=\Omega(g(n))$
- Big O is the commonly used one in industry

**Small o amd small $\omega$**

​															$$0 \le f(n) < cg(n)$$

​															$$0 \le g(n) < cf(n)$$

- Note that the inequality should not hold for *some* positive constant c, as is the case for big O/big $\Omega$ ; rather, it should hold for all positive constants.
- small o and small $\omega$ are *necessarily not tight*
- We can see that the small case notations are, in a sense, *relaxed* compared to their upper case notations.

Big $O$ notation denotes an upper bound (worst case) but is silent about the lower bound.

## Recursive

Merge sort is a typical recursive algorithm.

Binary search is recursive in nature but can also be implemented iteratively. 

Dynamic programming is similar to the divide-and-conquer with one important difference: The subproblems in divide-and-conquer are distinct and disjoint, whereas in the case of dynamic programming, the subproblems may overlap with one another. Also, dynamic programming problems can be solved in a bottom-up fashion instead of just a top-down approach

dynamic programming problems are usually optimization problems with several optimal solutions. "programming" means a *tabular* solution. Since dynamic programming problems involve subproblems that are overlapping or common, the answer to these subproblems can be stored in a table and reused when needed (memoization).

Top Down and Bottom Up approaches

Top Down

```java
class Demonstration {
    
    static int[] computed;
    
    public static void main( String args[] ) {
        long start = System.currentTimeMillis();

        int n = 50;
        computed = new int[n + 1];
        for (int i = 1; i < n + 1; i++)
            computed[i] = -1;

        System.out.println(StringSumCount(n));
        long end = System.currentTimeMillis();
        System.out.println("Time taken = " + (end - start));
    }

    static int StringSumCount(int n) {

        if (n == 0) {
            return 1;
        }

        if (n < 1) {
            return 0;
        }
        
        // If solution is already calculated
        if (computed[n] != -1)
            return computed[n];

        int sum = 0;

        // Number of ways to represent with 1
        sum += StringSumCount(n - 1);

        // Number of ways to represent with 2
        sum += StringSumCount(n - 2);

        // Number of ways to represent with31
        sum += StringSumCount(n - 3);

        computed[n] = sum;
        return sum;
    }  
}
```

Bottom up

```java
class Demonstration {
    public static void main( String args[] ) {
        long start = System.currentTimeMillis();
        int n = 50;
        System.out.println(StringSumCount(n));
        long end = System.currentTimeMillis();
        System.out.println("Time taken = " + (end - start));        
    }

    static int StringSumCount(int n) {

        int[] computed = new int[n + 1];
        computed[0] = 0;
        computed[1] = 1;
        computed[2] = 2;
        computed[3] = 4;

        for (int i = 4; i < n + 1; i++) {
            computed[i] = computed[i - 1] + computed[i - 2] + computed[i - 3];
        }

        return computed[n];
    }  
  
}
```

Merge sort by dividing the initial array into 3 subproblems instead of 2. Be caution to pass the boundaries for the three parts in the subsequent recursion portions.

```java
import java.util.Random;
import java.util.PriorityQueue;

class Demonstration {
    public static void main( String args[] ) {
        createTestData();
        long start = System.currentTimeMillis();
        mergeSort(0, input.length - 1, input);
        long end = System.currentTimeMillis();
        System.out.println("Time taken = " + (end - start));
        printArray(input);
    }

    private static int SIZE = 100;
    private static Random random = new Random(System.currentTimeMillis());
    static private int[] input = new int[SIZE];
    static PriorityQueue<Integer> q = new PriorityQueue<>(SIZE);

    private static void printArray(int[] input) {
        System.out.println();
        for (int i = 0; i < input.length; i++)
            System.out.print(" " + input[i] + " ");
        System.out.println();
    }

    private static void createTestData() {

        for (int i = 0; i < SIZE; i++) {
            input[i] = random.nextInt(10000);
        }
    }

    private static void mergeSort(int start, int end, int[] input) {

        if (start >= end) {
            return;
        } else if (start + 1 == end) {
            if (input[start] > input[end]) {
                int temp = input[start];
                input[start] = input[end];
                input[end] = temp;
            }

            return;
        }

        int oneThird = (end - start) / 3;

        // sort first half
        mergeSort(start, start + oneThird, input);

        // sort second half
        mergeSort(start + oneThird + 1, start + 1 + (2 * oneThird), input);

        // sort third half
        mergeSort(start + 2 + (2 * oneThird), end, input);

        // merge the three sorted arrays using a priority queue
        int k;

        for (k = start; k <= end; k++) {
            q.add(input[k]);
        }

        k = start;
        while (!q.isEmpty()) {
            input[k] = q.poll();
            k++;
        }
    }
}
```

unlike in traditional merge sort, the 3-way merge sort uses a priority queue to create a min-heap before attempting a merge of the three subproblems.

In professional settings, it is almost always preferable to write readable and maintainable code than to optimize for running time. It would appear that $log_{3}n$ is less than $lgn$ for large values values of n, however 3-way merge may not end up being faster than 2-way merge sort. Similar arguments can be made able binary search.

## Data Structures

### Array

*Contiguous memory allocation* is the key to an array's constant time retrieval/insert of an element given the index.

### Linked Lists

trade more time for less space

doesn't require contiguous allocation of memory. If we lose the head, we lost the linked list.

### Hash Table

A hash table or hash map is essentially a *key, value* pair. The has table use a *hash function* to compute the mapping.

use Array as a hash table (index as key).

The hash function - *if assumed to take constant time to compute the hash* isn't affected by the size of the table. Note, this assumption is crucial for the hash table to maintain constant time operations.

A **collision** is said to take place when two or more keys hash to the same slot. There are different ways to handle collisions, and each way will affect the complexity of different operations.

To mitigate the collision scenario, we can use a self-balancing binary search tree. Use hash tables provided by the standard libraries.

### Doubly Linked List

Usually, a doubly linked list is combined with a hash table to create a more advanced data structures called a least recently used cache or LRU cache.

### Stacks and Queues

Stack and Queue can be implemented using an array or a linked list

Stack (push, pop)

- using linked list: new items are always appended and removed at the head. *push* and *pop* operations take constant time
- using array: keep an integer pointer *top* to keep track of the top of the stack and increment it whenever a new push occurs. Poping elements is also a constant time operation.

Queue (enqueue, dequeue)

- using linked list: elements are always dequeued from the head and enqueued at the tail with two pointers. Both enqueue and dequeue operations take constant time
- using array: using front and end pointers to keep track of the head and tail. Both operations take constant time

Using linked list, the stack and queue can grow without bounds

### Tree Structures

**Tree Traversals**

The following recursive algorithms for tree traversals all take $O(n)$ time since we are required to visit each node

- pre-order traversal
- in-order traversal
- post-order traversal
- depth first search. Note the above three algorithms are types of depth first traversals
- breadth first traversal/search

The depth first traversal relies on the use of a stack. It is either implicit in case of recursion or explicit if an iterative version is written out.

**Height of Trees**

the longest path from the root to the leaf would be the height of the tree (between $log_2n$ and $n$)

**Trees becoming Linked Lists**

In case of a binary search tree, if it consists of all the nodes with the same values or inserts are made in an ascending order of values, then the binary search tree would turn into a linked list. The $O(lgn)$ promise for search is broken and search is now linear or $O(n)$ operation.

**Graph Traversals**

For graph traversals, the complexity would be $O(n)$ where n is the total number of nodes. Visiting each node once is the lease number of nodes you can visit to cover the entire graph. For a graph with cycles, you'll need to remember the visited nodes so as not to enter into an infinite cycle.

**Binary Trees**

A binary tree is a tree which has at most two children. Don't confuse a binary tree with a binary search tree. Searching in a binary tree will take $O(n)$, but in a binary search tree it'll take $O(lgn)$. However, an unbalanced binary search tree can still result in linear search.

We can store the binary tree in an array or create a class to represent each node of the tree

Using an array to store a binary tree makes access to any node of the  tree a constant time operation. However, if you choose to represent it  using the TreeNode approach then accessing the leaf nodes would take  time proportional to the height of the tree.

## Amortized Analysis

## Probabilistic Analysis

## Complexity Theory

## The End