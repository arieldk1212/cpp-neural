/*
 * demonstration of the std unique_ptr.
 */

#include <memory>
#include <utility>

template <typename T, typename U> class CompressedPair {
public:
  CompressedPair(U s) : second{s} {}
  CompressedPair(T f, U s) : first{f}, second{s} {}

private:
  [[no_unique_address]] T first;
  [[no_unique_address]] U second;
};

template <class T, class Del = std::default_delete<T>> class unique_ptr {
public:
  unique_ptr(T *ptr) : m_Pair(ptr) {}
  unique_ptr(T *ptr, Del deleter) : m_Pair(deleter, ptr) {}

  unique_ptr(const unique_ptr &) = delete;
  unique_ptr operator=(const unique_ptr &) = delete;

  unique_ptr(unique_ptr &&src)
      : m_Pair{std::exchange(src.m_Pair.second, nullptr)} {}
  unique_ptr &operator=(unique_ptr &&src) {
    m_Pair.second = std::exchange(src.m_Pair.second, m_Pair.second);
    m_Pair.first = std::exchange(src.m_Pair.first, m_Pair.first);
    return *this;
  }

  ~unique_ptr() {
    if (m_Pair.second) {
      m_Pair.first(m_Pair.second);
    }
  }

  T *operator->() { return m_Pair.second; }

private:
  CompressedPair<Del, T *> m_Pair;
};

template <typename T, auto DeleteFn>
using unique_ptr_deleter =
    std::unique_ptr<T, decltype([](T *obj) { DeleteFn(obj); })>;