#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <atomic>
#include <iterator>
#include <memory>
#include <utility>

/*
in order to make this kind of implementation there are few things to consider.
3. weak_ptr (test with breaking a circular refrence between two objects) - Not
Implemented!
5. enable_shared_from_this - Not Implemented!
6. benchmarks - Not Implemented!
*/

class control_block {
public:
  control_block()
      : ref_count(new std::atomic<int>(1)), weak_count(new std::atomic<int>) {}

  ~control_block() {
    if ((ref_count && ref_count->fetch_sub(1) == 1) &&
        (weak_count && weak_count->fetch_sub(1) == 1)) {
      delete ref_count;
      delete weak_count;
    }
  }

  int increment_ref() { return ref_count->fetch_add(1); }

  int decrement_ref() { return ref_count->fetch_sub(1); }

  int increment_weak() { return weak_count->fetch_add(1); }

  int decrement_weak() { return weak_count->fetch_sub(1); }

  int get_ref_count_int() const { return ref_count->load(); }

  int get_weak_count_int() const { return weak_count->load(); }

  std::atomic<int> *get_ref_count() { return ref_count; }
  std::atomic<int> *get_weak_count() { return weak_count; }

  void set_ref_count(std::atomic<int> *count) { ref_count = count; }
  void set_weak_count(std::atomic<int> *count) { weak_count = count; }

  // control_block &operator=(control_block&& other) {
  //   if (this == &other) {
  //     return *this;
  //   }
  //   ref_count = std::exchange(other.ref_count, nullptr);
  //   weak_count = std::exchange(other.weak_count, nullptr);
  // }

private:
  std::atomic<int> *ref_count;
  std::atomic<int> *weak_count;
};

template <typename T> class shared_ptr {
public:
  shared_ptr() : object(nullptr), ctrl_block() {}
  explicit shared_ptr(T *ptr) : object(ptr), ctrl_block(new control_block) {}

  void delete_shared_ptr() {
    if ((ctrl_block->get_ref_count_int() && ctrl_block->decrement_ref() == 1) &&
        (ctrl_block->get_weak_count_int() &&
         ctrl_block->decrement_weak() == 1)) {
      delete ctrl_block;
      delete object;
    }
  }
  ~shared_ptr() { delete_shared_ptr(); }

  /*
  ref_count->fetch_sub(1);
  (*ref_count).fetch_sub(1);
  shared_ptr &operator->() { return *this;} not quite correct.
  T *operator->() const { return object; } better because generic + returns a
  star. T &operator*() { return *object; }
  */

  shared_ptr &operator=(const shared_ptr &other) {
    if (this != &other) {
      delete_shared_ptr();
      object = other.object;
      ctrl_block->set_ref_count(other.get_control_block().get_ref_count());
      if (ctrl_block->get_ref_count_int()) {
        ctrl_block->increment_ref();
      }
    }
    return *this;
  }
  shared_ptr(const shared_ptr &other) {
    object = other.object;
    ctrl_block->set_ref_count(other.get_control_block().get_ref_count());
    ctrl_block->increment_ref();
  }

  shared_ptr &operator=(shared_ptr &&other) {
    if (this == &other) {
      return *this;
    }
    object = std::exchange(other.object, nullptr);
    ctrl_block->set_ref_count(other.get_control_block().get_ref_count());
    other.get_control_block().set_ref_count(nullptr);
    ctrl_block->set_weak_count(other.get_control_block().get_weak_count());
    other.get_control_block().set_weak_count(nullptr);
    return *this;
  }
  shared_ptr(shared_ptr &&other) {
    object = std::exchange(other.object, nullptr);
    ctrl_block->set_ref_count(other.get_control_block().get_ref_count());
    other.get_control_block().set_ref_count(nullptr);
    ctrl_block->set_weak_count(other.get_control_block().get_weak_count());
    other.get_control_block().set_weak_count(nullptr);
  }

  int get_count_for_ref() const { return ctrl_block->get_ref_count_int(); }
  int get_count_for_weak() const { return ctrl_block->get_weak_count_int(); }

  void increment_ref() { ctrl_block->increment_ref(); }
  void increment_weak() { ctrl_block->increment_weak(); }

  control_block &get_control_block() { return ctrl_block; }

  T *get() { return *object; }

private:
  T *object;
  control_block *ctrl_block;
};

template <typename T, typename... Ts>
shared_ptr<T> make_shared(Ts &&...params) {
  return shared_ptr<T>(new T(std::forward<Ts>(params)...));
}

template <typename T>
class weak_ptr {
public:
  weak_ptr(const shared_ptr<T> &other) {
    object = other.get();
    other.increment_weak();
  }
  ~weak_ptr() {
    delete object;
  }
private:
  T *object;
};

#endif