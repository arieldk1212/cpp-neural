#ifndef HFT_H_
#define HFT_H_

#include <atomic>
#include <pthread.h>
#include <string>
#include <vector>

/**
 * @todo read these repositories and play with them
 * @ref HFT Github Repository: https://github.com/stacygaudreau/nitek
 * @ref HFT Github Repository Original:
 https://github.com/PacktPublishing/Building-Low-Latency-Applications-with-CPP

 * @brief How do we achieve low-latency for HFT systems?
 1. Caching is incredibly important for low-latency, perfect world is staying at
 CPU cache layers and not memory bus (RAM). Trying to make data as fit as
 possible inside the CPU cache.
 We want our data to be contiguous memory blocks as possible, to avoid
 fragmantation.
 2. Algorithms and Data structures, for example unordered_map is linked list
 under the hood, meaning, heap allocated memory that is far from cpu cache and
 physical location, the memory is more fragmented therefore not cache-friendly.
 Let the compiler do alot of work BUT we have to give it hints, certain
 annotations (likely, unlikely - same line as the statement) across the program,
 it can improve the branch prediction when it tackles statements, if the
 compiler misses the branch, its costs alot to unwind the call stack, registers
 and so one.
 Virtual functions add overall complexity due to the fact that its run-time, we
 want to try and optimize the compiler's power to our hands.
 Try and avoid heap allocations.
 Multithreading, Can be misleading, more threads = more throughput, but not
 lower latency, alot of context switches can be costly, therefore there are some
 solutions to use multithreading more correctly: Pin threads to specific CPU
 cores, Avoid locks and context switching altogether, Develop custom thread
 pools (and memory pools on the stack) and threading constructs.
 Try to set compile-time stuff as much as you can! one way to replace virtual is
 to use CRTP, its similar, and its compile-time.
 */

/**
 * @brief Deep dives
 * Testing will be done with google test, it allows expected failures in tests,
 * unlike Catch2 which doesn't.
 */

/**
 * @brief Example one: Creating thread, starting it and pinning it to a CPU core
 */

auto PinThreadToCore(int core_id) {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(core_id, &cpu_set);
  return (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set) ==
          0);
}

template <typename T, typename... A>
auto CreateAndStartThread(int core_id, const std::string &name, T &&fn,
                          A &&...args) noexcept {
  std::atomic<bool> failed{false};
  std::atomic<bool> running{false};

  auto thread_body = [&]() {
    if (core_id >= 0 && !PinThreadToCore(core_id)) {
      failed = true;
      return;
    }

    running = true;
    std::forward<T>(fn)((std::forward<A>(args))...);
  };
  auto t = std::make_unique<std::thread>(thread_body);
  while (!(running || failed)) {
    std::this_thread::sleep_for(50ms);
  }
  if (failed) {
    t->join;
    t = nullptr;
  }
  return t;
}

/**
 * @brief Memory Pool
 * Due to the fact that at run-time heap allocations are really expensive in
 terms of latency, thats why we create a memory pool that is pre-initialized
 with the heap memory, allows us to easily allocate/deallocate objects on the
 block of memory.

  * the data structure of choice for the pool holder depends on the use-case, we
 can use array if the objects are small and temporary, vector for bigger objects
 with longer lifetime.

 * in the next implementation, our allocate() routine searches for the next
 available slot using .update_next_free_index(),
 the ideal solution is to make allocation/deallocation O(1), if we cant reach
 it, dont implement pool at all. The con is that we will have a chunck of memory
 that is completely isolated from the entire application, therefore it will be
 easier to debug, and error-prone if done correctly.
 */

// macro for the sake of simplicity
#define DELETE_DEFAULT_COPY_AND_MOVE(ClassName)                                \
public:                                                                        \
  ClassName() = delete;                                                        \
  ClassName(const ClassName &) = delete;                                       \
  ClassName(const ClassName &&) = delete;                                      \
  ClassName &operator=(const ClassName &) = delete;                            \
  ClassName &operator=(const ClassName &&) = delete;

template <typename T> class MemPool {
public:
  explicit MemPool(std::size_t n_block) : blocks_(n_block, {T(), true}) {}

  DELETE_DEFAULT_COPY_AND_MOVE(MemPool);

  auto update_next_free_index() noexcept {
    const auto i = i_next_free_;
    while (!&blocks_[i].is_free) {
      ++i_next_free_;
      if (i_next_free_ == blocks_.size()) [[unlikely]] {
        i_next_free_ = 0;
      }
    }
  }

  template <typename... Args> T *allocate(Args... args) noexcept {
    auto block = &blocks_[i_next_free_];
    auto object = &blocks_->object;
    object = new (object) T(args...);
    block->is_free = false;
    return object;
  }

  void deallocate(const T *object) noexcept {
    const auto i_object = reinterpret_cast<const Block *>(object) - &blocks[0];
    blocks[i_object].is_free = true;
  }

private:
  struct Block {
    T object; // the stored object
    bool is_free{true};
  };

private:
  size_t i_next_free_{0};
  std::vector<Block> blocks_;
};

/**
 * @brief How to deal with the context switches expensive price?
 * due to the fact that when we have a lot of threads and locks and mutexes to
 * handle how we transfer data between threads, all of that can be expensive
 * with context switching and what not. to solve this we implement a single
 * producer, single consumer pattern that will handle all the stuff that we want
 * between threads, the way to do that corrctly is with lock-free queues to
 * provide mechanism for sharing data between threads without having to use
 * locks.

 * @brief also loggin can be a bit of a overhead because writing log file to
 disk is also time consuming and adds latency, therefore we use a background
 thread to handle that for us using the lock free queue.
 */

#endif