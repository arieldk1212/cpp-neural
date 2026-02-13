#ifndef HFT_H_
#define HFT_H_

#include <atomic>
#include <pthread.h>
#include <string>

/**
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
  std::atomic<bool> running{false};
  std::atomic<bool> failed{false};

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

#endif