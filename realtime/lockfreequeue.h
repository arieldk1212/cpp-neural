#ifndef LOCK_FREE_QUEUE_
#define LOCK_FREE_QUEUE_

#include <atomic>
#include <exception>
#include <memory>
#include <new>
#include <vector>

/**
 * @brief lock free queue also known as ring buffer is a special kind of queue
 * that has the purpose of acting as a single consumer single producer queue
 * that is supposed to handle heavy traffic and provide a safe queue for
 * multithreaded environments.
 https://github.com/rigtorp/SPSCQueue/blob/master/include/rigtorp/SPSCQueue.h
 */

template <typename T, typename Allocator = std::allocator<T>>
class NeuralQueue {
public:
  NeuralQueue(std::size_t capacity, const Allocator &allocator = Allocator{})
      : data_(capacity, 0), capacity_(capacity), allocator_(allocator) {
    if (capacity_ == -1) {
      throw std::bad_exception();
    }
  }

private:
  /**
   * @brief we set read/write idx using alignas with the hardware size to
   * prevent false sharing (see false sharing).
   */
#ifdef __cpp_lib_hardware_interference_size
  static constexpr std::size_t kCacheLineSize =
      std::hardware_destructive_interference_size;
#else
  static constexpr std::size_t kCacheLineSize = 64;
#endif

private:
  T *slots_;
  Allocator allocator_;
  std::size_t capacity_;
  alignas(kCacheLineSize) std::vector<T> data_;
  alignas(kCacheLineSize) std::atomic<std::size_t> read_idx_{0};
  alignas(kCacheLineSize) std::atomic<std::size_t> write_idx_{0};
};

#endif