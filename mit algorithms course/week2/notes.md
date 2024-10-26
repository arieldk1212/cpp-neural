## Sets and Sorting
* upper bound -> proven algorithm that will use no more of a certain resource
* lower bound -> proven algorithm that will use no less of a certain resource
* resource can be memory, bandwidth, efficiency, and more.
* Omega sign is used to describe the lower bound of an algorithm's runtime complexity, how quickly it can possublt run (best-case).
* in contrast the Big O Notation is used to describe the upper bound of an algorithm's complexity.
* reduction -> mostly when talking recursive.
* substitution -> S(n) = Cn -> Cn for example is equal to C(n-1) + O(1) -> equal to O(1), that is the case for the certain example (selection sort).
* induction -> the way of proving a mathematical statemnet, in cs using recursive.
* substitution -> proving an asymptotic bound on a recurrence by induction, instead of trying to find an exact closed-form solution (finite number of operations) (one with simple algebraic expression, p.s non-closed-form is one that needs to be computed often through seccessuve approximation-infinite number of operations), we only try to find a closed-form bound on the recurrence.
* solve merge sort by using merge sort

## Hashing
* prove that you cant find(k) faster than O(logn)
* show how to find(k) faster than O(logn)
* the min height of the binary tree is Omega->O(logn)
* u = the largest key, u < 2^w, w is the size of the machine (64 bits)
