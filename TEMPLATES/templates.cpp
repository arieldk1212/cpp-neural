#include <iostream>
#include <string>

template <class T>
const T& max(const T& x, const T& y) {
  return (x >= y) ? x : y;
}

// in the compiler code we can see that it runs several times with different data types.

const char* max(const char* a, const char* b) {
  // if(strcmp(a, b) > 0) {
    return a;
  // } else {
    return b;
  // }
}

// class A {
// private:
//   int x;
// public:
//   A(int ax)
//     : x(ax) {}
// };

template <class T> class A {
private:
  T x;
public:
  A(const T& ax)
    : x(ax) {}
};

int main() {
  A<int> a(0);
  A<double> a(1.1);
}

template <class T>
class Vector {
private:
  T* m_arr;
  int m_size;
public:
  Vector(int size)
  : m_size(size) { m_arr = new T[size]; }
  Vector(const VEctor& a) {};
  ~Vector() { delete[]m_arr; }
  const Vector& operator=(const Vector& a);
  T& operator[](int index) { return m_arr[index]; }
  const void print();
};

template <class T> const void Vector<T>::print() {
  for (int i = 0; i < m_size; i++) {
    std::cout << m_arr[i] << "\n";
  }
}

// INHERITANCE

template <class T>
class A {
private:
  T x;
public:
 A (const T& ax) 
 :  x(ax)
};

class B1 : A<int> {
public:
  B1(int x)
  : A(x) {}
};

template <class T>
class B2 : A<T> {
public:
  B(const T& x)
  : A(x) {}
};