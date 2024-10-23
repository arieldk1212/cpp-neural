#include <iostream>
#include <string>
#include <mutex>


// creational pattern that lets you ensure that a class only has one instance
// while providing a global access point to this instance using static.
// it ensures that a class has just a single instance, it also provied a global
// access point to that instance.


// thread safe singleton example:
class Singleton {
public:
protected:
  Singleton(const std::string value)
    : value_(value) {}
  std::string value_;
private:
  static Singleton* pinstance_;
  static std::mutex mutex_;
};
