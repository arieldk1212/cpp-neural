#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template <typename T>
class Vector {
public:
  Vector(int size) {
    m_data = new T[size];
  };
  ~Vector() {
    delete[]m_data;
  };

  size_t size() const { return m_size; }



private:
  T* m_data = nullptr;;
  size_t m_size = 0;
  size_t m_capacity = 0;
};

#endif