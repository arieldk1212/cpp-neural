// think about const!
// every operator muist return something, not void.

// class Point {
// private:
//   int m_x, m_y;
// public:
//   Point(int x, int y)
//     : m_x(x), m_y(y) {};
//   Point operator+(const Point& p) const;
// };

// Point Point::operator+(const Point& p) const {
//   return Point(m_x + p.m_x, m_y + p.m_y);
// }

// int main() {


//   return 0;
// }

#include <iostream>
#include <ostream>
#include <istream>

class FVector {
private:
  float* items;
  int size;
public:
  FVector(int a_size) 
    : size(a_size){ items = new float[size]; }
  ~FVector(){ delete[]items; };

  // Operators
  float& operator[](int index);
  const FVector& operator=(const FVector& a);
  const FVector operator+(const FVector& a) const;

  // friend functions incase we dot use + on member of class, and the other is a member of the class.
  friend FVector operator+(const FVector& a, float val);
  friend FVector operator+(float val, const FVector& a);

  const FVector operator+=(float val);
  const FVector operator*=(float val);
  const FVector operator++();
  const FVector operator++(int); //for postfix

  const int operator==(const FVector& a);
  const int operator!=(const FVector& a);

  const FVector& operator()(float* arr, int s_size);

  friend std::ostream& operator<<(std::ostream& out, const FVector& a);
  friend std::istream& operator>>(std::istream& out, FVector& a);

  operator const float*() const { return items; };
  const FVector operator-() const;
};

const FVector& FVector::operator=(const FVector& a) {
  if (&a != this) {
    delete[]items;
    size = a.size;
    items = new float[size];
    for (int i = 0; i < size; i++) {
      items[i] = a.items[i];
    }
  }
  return *this;
}

const FVector FVector::operator+(const FVector& a) const {
  FVector temp_vector(size + a.size);
  for (int i = 0; i < size; i++) {
    temp_vector.items[i] = items[i];
  }
  for (int i = 0; i < a.size; i++) {
    temp_vector[size + i] = a.items[i];
  }
  return temp_vector;
}

FVector operator+(const FVector& a, float val) {
  FVector temp_vector(a.size);
  for (int i = 0; i < a.size; i++) {
    temp_vector[i] = a.items[i] + val;
  }
  return temp_vector;
}

FVector operator+(float val, const FVector& a) {
  return a + val;
}

const FVector FVector::operator+=(float val) {
  for (int i = 0; i < size; i++) {
    items[i] += val;
  }
  return *this;
}

const FVector FVector::operator*=(float val) {
  for (int i = 0; i < size; i++) {
    items[i] *= val;
  }
  return *this;
}

// prefix - incremented by 1 then returns the value.
const FVector FVector::operator++() {
  (*this) += 1.0;
  return *this;
}

// postfix - returns the value, then incremented by 1
const FVector FVector::operator++(int) {
  FVector temp_vector = *this;
  (*this) += 1.0;
  return temp_vector;
}

const int FVector::operator==(const FVector& a) {
  if (a.size != size) {
    return false;
  } 
  for (int i = 0; i < size; i++) {
    if (items[i] != a.items[i]) {
      return false;
    }
  } return true;
}

const int FVector::operator!=(const FVector& a) {
  return !(*this == a);
}

const FVector& FVector::operator()(float* arr, int s_size){
  if (items != NULL) {
    delete[]items;
  }
  size = s_size;
  items = new float[size];
  for (int i = 0; i < size; i++) {
    items[i] = arr[i];
  } return *this;
}

std::ostream& operator<<(std::ostream& out, const FVector& a) {
  for (int i = 0; i < a.size; i++) {
    out << "item" << i << "=" << a.items[i];
  } return out;
}

std::istream& operator>>(std::istream& in, FVector& a) {
  std::cout << "Please Enter " << a.size << " Numbers.";
  for (int i = 0; i < a.size; i++) {
    in >> a[i];
  } return in;
}

int main() {

  return 0;
}