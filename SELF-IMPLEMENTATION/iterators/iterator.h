#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>

template <typename IteratorType> class generic_iterator : public IteratorType {
private:
  IteratorType *m_IteratorData;
  size_t m_IteratorCapacity;

private:
  struct Iterator {
    using IterPtr = IteratorType *;

    Iterator(IteratorType ptr) : m_IteratorPtr(ptr) {}

    IterPtr m_IteratorPtr;
  };

public:
  generic_iterator(int capacity) : m_IteratorCapacity(capacity) {
    m_IteratorData = new IteratorType[capacity];
  }
  ~generic_iterator() {
    if (m_IteratorData != nullptr) {
      delete[] m_IteratorData;
    }
  };

  size_t GetIteratorCapacity() { return m_IteratorCapacity; }

  IteratorType &operator[](size_t index) { return m_IteratorData[index]; }

  IteratorType &operator!=()

  Iterator begin() { return Iterator(&m_IteratorData[0]); }

  Iterator end() { return Iterator(&m_IteratorData[m_IteratorCapacity]); }
};

#endif