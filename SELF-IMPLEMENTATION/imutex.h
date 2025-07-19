#ifndef IMUTEX_H
#define IMUTEX_H

#include <atomic>
#include <mutex>

class IMutex {
public:
  IMutex() : flag(ATOMIC_FLAG_INIT) {}
  ~IMutex() = default;

  void lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
    }
  }

  void unlock() { flag.clear(std::memory_order_release); }

private:
  std::atomic_flag flag;
};

#endif