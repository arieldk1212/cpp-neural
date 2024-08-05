#include <iostream>
#include <stdlib.h>

// SECTION 1

// void change_value(int *nb);

// int main() {

//   int nb = 42;
//   change_value(&nb);
//   // std::cout << nb << "\n";

//   int x = 2;
//   int *ptr = &x;
//   *ptr = 5;
//   // std::cout << x << "\n";
//   // std::cout << ptr << "\n";
//   // std::cout << *ptr << "\n";

//   int y = 3;
//   int *ptrr;
//   ptrr = &y;
//   *ptrr = 8;
//   // std::cout << y << "\n";
  
//   int n;
//   int *pn;
//   int **ppn;
//   int ***pppn;
//   n = 42;
//   pn = &n;
//   ppn = &pn;
//   pppn = &ppn;


//   return 0;
// }

// void change_value(int *nb) {
//   *nb = 43;
// } 

// SECTION 2

// void bar() {
//   int nb; 
//   nb = 1337;
// }

// int *foo() {
//   // OLD
//   // int n;
//   // n = 42;
//   // return &n;

//   // NEW
//   int *n;
//   // n = malloc(sizeof(int)); // we dont write malloc in cpp.
//   *n = 42;
//   return n;
// }

// int main () {
//   int *pn;
//   pn = foo();
//   std::cout << *pn << "\n";
//   bar();
//   std::cout << *pn << "\n";
// }
// what happens here is that the final result with the pointer pn is 1337, why? because it is using the same address as the value 42, its overwriting it!
// to fix this issue we need to declare malloc, to clear some space for our pointer that other data will not be ovewritten.

// SECTION 3


// int main() {

//   int arr[10];
//   // arr == &arr[10] in mem, meaning -> arr is kind of a pointer..(void)

//   int v[10];
//   v[1] = 42;
  // *(v + 1) == v[1]

  // arr[5] == 5[arr]; -> because [ ] == arr[n] == *(arr + n) 
  // arr[5] == *(arr + 5)
  // 5[arr] == *(5 + arr)

//   return 0;
// }


// SECTION 4

int main() {
  int nb = 20;
  int *ptr1 = &nb;
  int **ptr2 = &ptr1;
  std::cout << ptr2 << "\n";
  std::cout << **ptr2 << "\n";
  std::cout << *ptr2 << "\n";
  std::cout << ptr1 << "\n";
  std::cout << *ptr1 << "\n";
  std::cout << &ptr1 << "\n";

  return 0;
}