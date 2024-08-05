1. in mem we put data types, meainng ram, 1 byte = 8 bits... etc.
2. char=1, short=2, int=4, float=4, long=8, double=8 (bytes)

* PASS-BY-VALUE:
  - easy to read
  - safe
  - lackmof direct access
  - performance overhead(copying struct?... not recommended)

* PASS-BY-REFRENCE
  - Efficiency
  - Direct access
  - Complexity
  -  Unreadable code

* *(float *)ptr; -> this means, that if originally ptr is a pointer to an int, this time we are casting him to be a pointer to a float number.
* can do the same for all types... *(char *)ptr;
* void==generic.. - can be anything, for example:
  - void *ptr
  - *(char *)ptr;
  - *(int *)ptr;

* malloc -> "gives" some memory in the heap, can be translated to a void ptr (can say generic pointer)..