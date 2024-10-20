## Correctness
* Programs/algorithms have fixed size, so how to prove correct?
* For small inputs, can use case analysis
* For arbitrarily large inputs, algorithm must be recursive or loop in some way
* Must use induction (why recursion is such a key concept in computer science)

## Efficiency
* could measure time, but want performance to be machine independent
* Idea! Count number of fixed-time operations algorithm takes to return
* Expect to depend on size of input: larger input suggests longer time
* Size of input is often called ‘n’, but not always!
* Efficient if returns in polynomial time with respect to input
* Sometimes no efficient algorithm exists for a problem! (See L20) 

## Data Structure
* • A data structure is a way to store non-constant data, that supports a set of operations

## How to solve an Algorithms Problem
*  Reduce to a problem you already know (use data structure or algorithm)
* like: seach problems, sort algorithms, shortest path algorithms.
* design your own (recursive) algorithm: brute force, decrease and conquer, divide and conquer, dynamic programming, greed / incremental.

# Explanations
* divide and conquer -> divide the problem, into sub problems, until it meets an "easy" way to conquer the problem, breaking it down meaning after the division, the sub problem is most likely to be generic in the way of solving the base problem, need to be done RECURSIVLEY.
* decrease and conquer -> we decrease not the problem, but the instance itself.
* dynamic programming -> breaking down into sub problems, solving them and STORING the result, to record the way we implemented the answer to the problem.