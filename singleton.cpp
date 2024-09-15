#include <iostream>

// kinda a static class, no need to initialize it, its purpose servers only static functions and members.
// we have to first type get() because it returns the instance that is currently being dealt.

// if we have a function that just returns a number or so, we can type it a static function.
// you can use a namespace instead, but its more organized, and with class we can assign the class instance to a variable.


// class Singleton {
// public:
//   Singleton(const Singleton&) = delete;

//   static Singleton& get() {
//     return s_instance;
//   }

//   void function();
// private:
//   Singleton() {}
//   static Singleton s_instance;
// };

// Singleton Singleton::s_instance;

// int main() {
//   auto& instance = Singleton::get();
//   Singleton::get();
//   instance.function();
//   Singleton& isntance2 = Singleton::get();
//   // NOT VALID -> Singleton instance3 = Singletom::get();
// }

class Random {
public:
  Random(const Random&) = delete;

  static Random& Get() {
    static Random instance;
    return instance;
  }

  static float Float() { return Get().IFloat(); }
private:
  float IFloat() { return m_randomgenerator; }
  Random() {}
  float m_randomgenerator = 0.5f;
};