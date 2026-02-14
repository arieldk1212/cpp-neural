#ifndef MEMORY_ALLOCATION_H_
#define MEMORY_ALLOCATION_H_

#include <__new/global_new_delete.h>
#include <limits>
#include <memory>
#include <new>

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
 1. Pool Allocator - good at recycling objects of the same size, eliminates
 memory fragmentation for object type, BUT it only works on a fixed size,
 reserves a large block of memory upfront, BEST FOR nodes in a list, tree,
 graph, game object like bullets and enemies, any system that create or destroy
 many object of the same size.
 2. Stack Allocator - ultra fast and temporary (it DOESNT mean its on the stack!
 it resembles a stack data strcuture), it can allocate multiple different types
 of objects! good for moving a single ptr to free, and good for memory
 fragmantation because there is zero as you dont fragmant meaning you can handle
 big objects, BUT you cant deallocate what was at the start beacuse of the stack
 traits - individual out of order objects, the stack allocator is difficult for
 unpredictable lifetimes, BEST FOR game engines like allocating memory to handle
 a single frame, short-lived data, scratchpad memory.

 TO:SUM:
 Allocation: both designs are O(1), stack a little faster.
 Deallocation: Pool - Granular, Stack - Scoped (all at once).
 Flexibility: Pool - fixed-size objects, Stack - Any-size objects.
 Best For: Pool - Reusable objects, Stack - Temporary data.
 */

// Pool Allocator Example
template <typename T> class NeuralPoolAllocator {
public:
  NeuralPoolAllocator() { initialize_pool(); }

  static constexpr int PoolSize{56};

  T *allocate() {
    if (free_head_ == nullptr) {
      throw std::bad_alloc();
    }

    T *res = reinterpret_cast<T>(free_head_); // get current block of memory
    free_head_ = free_head_->next;            // move to the next block
    ++allocated_count_; // increase the number of allocation made so far

    return res;
  }

  void deallocate(T *p) {
    if (p == nullptr) {
      return;
    }
    if (p < reinterpret_cast<T *>(pool_) ||
        p >= reinterpret_cast<T *>(pool_ + sizeof(T))) {
      return; // not from this pool!
    }

    FreeNode *node = reinterpret_cast<FreeNode *>(p);
    node->next = free_head_;
    free_head_ = node;
    --allocated_count_;
    // we are not getting the memory back, we are simply "kicking" the object
    // out from that block, we return the memory back to the POOL.
  }

private:
  struct FreeNode {
    FreeNode *next;
  };

  void initialize_pool() {
    char *current = pool_;
    for (std::size_t i{0}; i < PoolSize; ++i) {
      reinterpret_cast<FreeNode *>(current)->next =
          reinterpret_cast<FreeNode *>(
              current +
              sizeof(
                  T)); // setting the next to a new block of the same size as T
      current += sizeof(
          T); // moving forward, setting current location to the next block
    }
    reinterpret_cast<FreeNode *>(current)->next =
        nullptr; // last block is nullptr
    free_head_ =
        reinterpret_cast<FreeNode *>(pool_); // set free head to the first
  }

  FreeNode *free_node_{nullptr};
  FreeNode *free_head_{nullptr};
  std::size_t allocated_count_{0};
  alignas(T) char pool_[PoolSize * sizeof(T)];
};

using NeuralAllocatorTraits =
    std::allocator_traits<NeuralPoolAllocator<int>>; // can replace int with the
                                                     // objec itself..

// Stack Allocator Example
template <typename T> class NeuralStackAllocator {
public:
  explicit NeuralStackAllocator(std::size_t size)
      : total_size_{size}, current_offset_{0} {
    memory_ = new char[size];
  }

private:
  char *memory_;               // ptr to the allocated memory block
  std::size_t total_size_;     // total size of the memory block
  std::size_t current_offset_; // current top (next allocation position)
};

// Tracking Allocator Example - Real Life
// Without Relying on Allocator Traits
template <typename T> class TrackingAllocator {
public:
  using value_type = T;

  TrackingAllocator() = default;

  template <typename U>
  constexpr TrackingAllocator(const TrackingAllocator<U> &) noexcept {}

  // must have for allocators - but allocator traits fiil it by default
  bool operator==(const TrackingAllocator<T> &) const { return true; }
  bool operator!=(const TrackingAllocator<T> &) const { return true; }

  T *allocate(std::size_t n) {
    return static_cast<T *>(::operator new(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t n) { ::operator delete(p); }
};

/**
 * @brief Allocator - Real World Benefits
 Gaming: Pool allocators for bullets, particles can eliminate the stutter of
 new.
 Trading: Stack allocators for market data messages help ensure low latency.
 Embedded: Fixed memory blocks can help prevent fragmentation and reliability.
 */

#endif