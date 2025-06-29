#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template <typename T> class unique_ptr {
public:
  unique_ptr() : object(nullptr) {}
  unique_ptr(T *ptr) : object(ptr) {}
  void delete_ptr() {
    delete object;
    object = nullptr;
  };
  ~unique_ptr() { delete_ptr(); }

  unique_ptr &operator=(const unique_ptr &) = delete;
  unique_ptr(const unique_ptr &) = delete;

  unique_ptr &operator=(unique_ptr &&other) {
    if (*this == other) {
      return *this;
    }
    delete_ptr();
    object = std::exchange(other.object, nullptr);
    return *this;
  }
  unique_ptr(unique_ptr &&ptr) { object = std::exchange(ptr.object, nullptr); }

  void reset() { delete_ptr(); }

  void reset(T *other) {
    delete_ptr();
    object = other;
  }

  T *release() { return std::exchange(object, nullptr); }

private:
  T *object;
};

// universal refrence...
template <typename T, typename... Ts>
unique_ptr<T> make_unique(Ts &&...params) {
  return unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

#endif