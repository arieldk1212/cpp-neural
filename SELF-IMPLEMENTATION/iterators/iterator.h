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
    ~Iterator() {
      if (m_IteratorPtr != nullptr) {
        delete m_IteratorPtr;
      }
    }

    IteratorType *operator->() { return m_IteratorPtr; }

    IteratorType &operator*() const { return *m_IteratorPtr; }

    Iterator &operator++() {
      ++m_IteratorPtr;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief friend for allowing to access iterator private member, also we
     * (but usage with a, b. not other) dont need to define != because the
     * compiler should be samrt enough, but will do anyway.
     */
    bool operator==(const Iterator &other) const {
      return m_IteratorPtr == other.m_IteratorPtr;
    }

    bool operator!=(const Iterator &other) const {
      return m_IteratorPtr != other.m_IteratorPtr;
    }

  private:
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

  Iterator begin() { return Iterator(&m_IteratorData[0]); }

  Iterator end() { return Iterator(&m_IteratorData[m_IteratorCapacity]); }
};

#endif