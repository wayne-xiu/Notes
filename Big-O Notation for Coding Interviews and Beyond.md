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



## Data Structures

## Amortized Analysis

## Probabilistic Analysis

## Complexity Theory

## The End