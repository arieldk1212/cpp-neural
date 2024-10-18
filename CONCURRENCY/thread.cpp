#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

/* RAII */
// RAII - RESOURCE ACQUSITION IS INITIALIZATION -> mutex, atomic, smart ptrs, heap mem,
          //sockets, files, connection, everything that has a lifetime.

/* MUTEX*/
// mutex - mutually exclusive access, one resource has an exclusive access to a certain resource.
// i.e. a resource has a "key" to the resource "door" (a certain thread).
// mutex insures that ONLY one thread is using the resource no matter if the resource
// is not defined properly lik you have to do in atomic (i.e. resource++ and not
//  resource = resource + 1), the resource in mutex is indeed shared across threads,
// and its in sync, it adds protection to the resource from being simultaneously accessed
// by multiple threads -> OWNERSHIP SEMANTICS.
// lock -> locked. until unlock is called.

/* MUTEX - TYPES OF LOCKS */
// std::lock_guard -> a way to handle lock and unlock, saves us the lines and the need to 
// type unlock at the end of the mutex, its defined like so:
//    std::lock_guard<std::mutex> lock_guard_me(glock); | glock is in the main not commented.
// std::unique_lock -> more advanced type of lock, handled also by itself, the prefered one.
// std::scoped_lock -> is for multiple mutexes.

/* ATOMIC */
// atomic - is a value that can be changed by different threads.
// but each thread (when handled correctly) changes the value at a time,
// insuring that the value will be treated the way we want.
// basically a shared value acroos threads.
/* ATOMIC - SUM UP */
// to sum up atomic -> one thread writes, the other read from the resource, if thats true this
// behavior is "well-defined". it makes sure object may establish inter-thread sync
// and order non-atomic memory accesses.

/* GENERAL */
// atomic is mostly for counters and small operations
// mutex is more big scaled, for objects and very much more.

/* EXAMPLES */
std::mutex glock;
static int mutex_shared_value = 0;

static std::atomic<int> shared_value = 0;

void increment_atomic() { shared_value++; std::cout << "shared is: " << shared_value << "\n"; }
void increment_mutex() {
  glock.lock();
  mutex_shared_value = mutex_shared_value + 1;
  // can also type mutex_shared_value++;
  glock.unlock();
  // OR
  // std::lock_guard<std::mutex> lock_guard_me(glock);
  // mutex_shared_value = mutex_shared_value + 1;
  std::cout << "mutex shared is: " << mutex_shared_value << "\n";
}

void test_thread(int x) {
  std::cout << "threaded func.." << std::this_thread::get_id() << "\n";
  std::cout << "the number is " << x << "\n";
}

int main() {
  // in c++20 consider jthread, joining thread by default, making it a safter environment
  // if using thread and forgetting about the join part, the program would crash,
  // thus creating alot of bugs throughtout the code base.

  // std::jthread j_thread(test_shared, 10);
  std::vector<std::thread> threads;
  std::vector<std::thread> threads_atomic;
  std::vector<std::thread> threads_mutex;
  for (int i=0; i <1000; i++) {
    threads.push_back(std::thread(test_thread, i));
    threads_atomic.push_back(std::thread(increment_atomic));
    threads_mutex.push_back(std::thread(increment_mutex));
  }
  for (int i=0; i <10; i++) {
    threads[i].join();
    threads_atomic[i].join();
    threads_mutex[i].join();
  }
  // std::thread thread(test_thread, 10);
  // if we dont pass the join, it will ouput aborted
  // it just combines the main thread and the seperate thread.
  // thread.join();

  return 0;
}