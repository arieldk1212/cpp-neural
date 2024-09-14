#include "vector.h"
#include <iostream>

template<typename T>
void printvector(const Vector<T>& vector) {
  for (size_t i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << "\n";
  }
}

int main () {
  Vector<std::string> vector;
  vector.push_back("ariel");
  vector.push_back("david");
  vector.push_back("kriheli");

  printvector(vector);
};