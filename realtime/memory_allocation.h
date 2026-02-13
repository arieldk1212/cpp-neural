#ifndef MEMORY_ALLOCATION_H_
#define MEMORY_ALLOCATION_H_

#include <limits>
#include <memory>

/**
 * @brief Why should we create a custom memory allocator?
 1. make the allocation instantaneous, instead of using the regular new (after
 new comes malloc, then context switch, the kernel brk and mmap then another
 swtich, then object creation and so on..).
 2. eliminate memory fragmentation for the key data strcutures (custom made).
 3. to lay out the data perfectly for the cpu cache.
 NOTE: we can always define a custom allocator and then create a method to
 destroy all heap memory.
 NOTE: alignas lets you set where a specific object should be started in memory.
 alignof lets you check whats the value of alignas for the object (Alignment of
 16 means that memory addresses that are a multiple of 16 are the only valid
 addresses.) THEN: assert(alignof(a) == 16);
 */

/**
 * this: std::vector<int> vec;
 * equals to: std::vector<int, std::allocator<int>> vec;
 * std::allocator isnt used everyday, but it is used in STL, its the modern way
 * of controlling memory, its extensible and flexible.
 */

/**
 * @brief basic usage of std::allocator (the built in stl allocator)
 std::allocator<int> allocator;
 int *arr = allocator.allocate(5);

 for (...) {
    std::construct_at(&arr[i], i * 10);
  }

  for (...) { std::destroy_at(&arr[i]); }

  std::deallocate(arr, 5);
 */

// possible implementation of std::allocate
template <typename T> T *neural_allocate(std::size_t n) {
  if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
    return; // bad alloc
  }

  void *p = ::operator new(n);
  return static_cast<T>(p);
}

// possible implementation of std::deallocate
template <typename T> void neural_deallocate(T *p, std::size_t n) {
  ::operator delete(p);
}

// allocator implementation
template <typename T> class neural_allocator {
public:
  using value_type = T;

  [[nodiscard]] T *allocate(std::size_t n) { return neural_allocate<T>(n); }

  void deallocate(T *p, std::size_t n) { neural_allocator<T>(p, n); }

private:
};

/**
 * @brief allocator traits
 * in the modern c++ we use std::allocator_traits<T>::construct to let the
 * compiler add default and parts missing from our implementation, therefore
 * helping us advance, we only need to implement allocate and deallocate and
 * thats it.
 */

/**
 * @brief Allocator Designs
 * there are two main types of allocator designs:
 1. Pool Allocator - good at recycling objects of the same size.
 2. Stack Allocator - ultra fast and temporary.
 */

// Pool Allocator Example
template <typename T> class NeuralPoolAllocator {
public:
  static constexpr int PoolSize{56};

private:
  struct FreeNode {
    FreeNode *next;
  };

  alignas(T) char pool_[PoolSize * sizeof(T)];

  FreeNode *free_node_{nullptr};
  std::size_t allocated_count_{0};
};

#endif