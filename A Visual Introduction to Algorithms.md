# A Visual Introduction to Algorithms

[toc]



## Intro to Algorithms

Algorithms put "science" into "computer science".

- Compression algorithms
- Route finding algorithms
- Rending algorithms
- Optimization & Scheduling algorithms
- Minimax algorithms

What makes a good algorithm?

1. Correctness
2. Efficiency

Perfect or good enough

Asymptotic analysis

*binary search* for sorted arrays

Route-finding: Start on the goal, keep marking squares in the maze in order of increasing distance from the goal (k->k+1). The program then find a path to the goal by choosing path of decreasing, like going downhill.

## Binary Search

Repeatedly dividing in half the portion of the list

```c++
int binarySearch(vector<int>& arr, int goal) {
    size_t min = 0;
    size_t max = arr.size()-1;
    size_t mid;
    while (min <= max) {
        mid = (min+max)/2;
        if (arr[mid] < goal)
            min = mid + 1;
        else if (arr[mid] > goal)
            max = mid - 1;
        else if (arr[mid] == goal)
            return mid;
    }
    return -1;
}
```

Binary search time complexity: $log_2(n)$

## Asymptotic Analysis

- big-$\Theta$: an asymptotically tight bound on the running time
- big-O: asymptotic upper bounds (may be worse than worst case)
- big-$\Omega$: asymptotic lower bounds (run time at least)

Note that exponential function $a^n$ where (a>1) grows faster than any polynomial $n^b$

big-$\Theta$ notation asymptotically bound the growth of a running time to within constant factors above and below.

Because big-O notation gives only an asymptotic upper bound, and not an  asymptotically tight bound, we can make statements that at first blush  seem incorrect, but are technically correct. For example, it is  absolutely correct to say that binary search runs in **O(n)** time. That's because the running time grows no faster than a constant times n. In fact, it grows slower. 

## Selection Sort

swap function

Find minimum in subarray

Selection sort time complexity: $\Theta(n^2)$

## Insertion Sort

Sum up the running times for insertion sort:

- worst case: $\Theta(n^2)$
- Best case: $\Theta(n)$
- Average case for random array: $\Theta(n^2)$
- "Almost sorted" case: $\Theta(n)$

note: the worst case for insertion sort is a reverse-sorted array

## Recursion Algorithms

