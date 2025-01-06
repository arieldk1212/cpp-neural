#ifndef VECTOR_H
#define VECTOR_H

template<typename T>
class Vector {
public:
  Vector() {
    ReAlloc(2);
  };

  ~Vector() { clear(); ::operator delete(m_data, m_capacity * sizeof(T)); }

  void push_back(const T& value) {
    if (m_size >= m_capacity) {
      ReAlloc(m_capacity + m_capacity / 2);
    }
    m_data[m_size] = value;
    m_size++;
  }

  void push_back(T&& value) { // for rvalue
    if (m_size >= m_capacity) {
      ReAlloc(m_capacity + m_capacity / 2);
    }
    m_data[m_size] = std::move(value);
    m_size++;
  }

  template<typename... Args>
  T& emplace_back(Args&&... args) {
    if (m_size >= m_capacity) {
      ReAlloc(m_capacity + m_capacity / 2);
    }
    new(&m_data[m_size]) T(std::forward<Args>(args)...); // for constructing structures in place.
    return m_data[m_size++];
  }

  void pop_back() {
    if (m_size > 0) {
      m_size--;
      m_data[m_size].~T();
    }
  }

  void clear() {
    for (size_t i = 0; i < m_size; i++) {
      m_data[i].~T();
    } m_size =  0;
  }

  const T& operator[](size_t index) const {
    return m_data[index];
  }

  T& operator[](size_t index) {
    return m_data[index];
  }

  size_t size() const { return m_size; }
private:
  void ReAlloc(size_t new_capacity) {
    T* new_block = (T*)::operator new(new_capacity * sizeof(T));

    for (size_t i = 0; i < m_size; i++) {
      new_block[i] = std::move(m_data[i]);
    }

    for (size_t i = 0; i < m_size; i++) {
      m_data[i].~T();
    }

    ::operator delete(m_data, m_capacity * sizeof(T));
    m_data = new_block;
    m_capacity = new_capacity;
  }

private:
  T* m_data = nullptr;
  size_t m_size = 0;
  size_t m_capacity = 0;
};


#endif