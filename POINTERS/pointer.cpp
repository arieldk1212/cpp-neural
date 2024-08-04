#include <iostream>

void change_value(int *nb);

int main() {

  int nb = 42;
  change_value(&nb);
  // std::cout << nb << "\n";

  int x = 2;
  int *ptr = &x;
  *ptr = 5;
  // std::cout << x << "\n";
  // std::cout << ptr << "\n";
  // std::cout << *ptr << "\n";

  int y = 3;
  int *ptrr;
  ptrr = &y;
  *ptrr = 8;
  // std::cout << y << "\n";
  
  int n;
  int *pn;
  int **ppn;
  int ***pppn;
  n = 42;
  pn = &n;
  ppn = &pn;
  pppn = &ppn;


  return 0;
}

void change_value(int *nb) {
  *nb = 43;
} 