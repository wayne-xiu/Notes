# Data Structures in C++: An Interview Refresher

[toc]

## Introduction to Complexity Measures

time and space

primitive operation

- Best case analysis - lower bound of execution time
- Worst case analysis - upper bound of execution time
- Average case analysis - compute the weighted average of the number of operations with the relative frequencies

time complexity

Asymptotic analysis and Big O

cares about large input sizes only

A function $f(n)$ is considered $O(g(n))$ if there exists some positive real constant c and an integer $n_0\geq 0$ such that the following inequality holds for all $n \geq n_0$: $f(n) \leq cg(n)$

$O(g(n))$ is a set of functions

When dealing with time and space complexities, we are generally interested in the tightest possible bound when it comes to the asymptotic notation.

![commonFunctions](Media/commonFunctions.png)

The above table lists some commonly encountered functions in ascending order of rate of growth. Any function can be given as Big O of any other function that appears later in this table.

Other common Asymptotic notations

- Big 'Omega' - $\Omega$: a function $f(n)$ is in $\Omega(g(n))$ if $f(n)$ will grow at least as fast as $g(n)$

> It is a common misconception that Big O characterizes worst case running time while Big Omega characterizes best case running time of an algorithm. There is no one-to-one relationship between any of these cases and the asymptotic notations.

if $f(n) \in O(g(n))$ then $g(n) \in \Omega(f(n))$

- Big 'Theta' - $\Theta$: if $f(n)$ is in $O(g(n))$ and $f(n)$ is also in $\Omega(g(n))$ then it is in $\Omega(n)$; If $f(n)$ is $\Theta(g(n))$, then the two functions grow at the same rate, within constant factors

