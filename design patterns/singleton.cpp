#include <iostream>
#include <string>
#include <mutex>
#include <thread>


// creational pattern that lets you ensure that a class only has one instance
// while providing a global access point to this instance using static.
// it ensures that a class has just a single instance, it also provied a global
// access point to that instance.


// thread safe singleton example:
class Singleton {
public:
  Singleton(Singleton& other) = delete;
  void operator=(const Singleton&) = delete;
  static Singleton* get_instance(const std::string& value);
  void some_business_logic() {
    // ...
  };
  std::string value() const {
    return value_;
  }
protected:
  Singleton(const std::string value)
    : value_(value) {}
  ~Singleton() {}
  std::string value_;
private:
  static Singleton* pinstance_;
  static std::mutex mutex_;
};

Singleton* Singleton::pinstance_{nullptr}
std::mutex Singleton::mutex_;

Singleton* Singleton::get_instance(const std::string& value) {
  std::lock_guard<std::mutex> lock(mutex_);
}

void ThreadFoo() {
  // inits a slow thread function.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  Singleton* singleton = Singleton::get_instance("BAR");
  std::cout << singleton->value() << "\n";
}

void ThreadBar() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  Singleton* singleton = Singleton::get_instance("BAR");
  std::cout << singleton->value() << "\n";
}
int main () {
  std::cout << "if you see the same value then singleton was reused!! good!!!if not not good fix something.\n";
  std::thread t1(ThreadFoo);
  std::thread t2(ThreadBar);
  t1.join();
  t2.join();
}  
