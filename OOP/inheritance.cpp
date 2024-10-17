// logical specification
// defining something more specific
// extending its pysical volume and functionality.
// methods overriding
// the inherited class gets constructed once the lower class get constructed (in our case once B is constructed, A is also constructed).

// in inheritance, if you init from the base calls a ptr to a new derived class, a virtual table (vtable __vtbl) inside mem is initislized
// and is pointing to the methods which the programmer is refrencing.

#include <iostream>

class A {
private:
  int x;
public:
  A() { std::cout << "A"; }
  A(int ax) { x = ax; }
  int y;
  void f() { std::cout << "this is a::f"; };
  void m() { std::cout << "this is a::m"; };
  void set_x(int ax) { x = ax; };
};

class B : public A {
private:
  int y; // hides A::y;
  int x;
  A a; // A Ctor will gets executed twice because we created here an A class instance.
public:
  B() { std::cout << "B"; } // when B gets initialized, A also gets.
  B(int x1, int x2, int x3)
    : A(x1), a(x2) {x=x3;}
  void z() { std::cout << "this is b::z"; };
  void m() { std::cout << "this is b::m"; };
  void g() {
    A::m;
    m();
  }
  void init() {
    y = 2; // B::y;
    A::y = 1;
    // x = 0 -> error x is private.
    set_x(5);
  }
};


class Stack {
private:
  int* m_arr;
  int m_size;
  int m_pos;
public:
  Stack(int size)
  : m_size(size) { m_arr = new int[m_size]; m_pos = 0; }
  ~Stack(){ delete[]m_arr; }
  Stack(const Stack& s) {
    m_arr = NULL;
    *this = s;
  }
  const Stack& operator=(const Stack& s) {
    return *this;
  }
  void Push(int val) {
    if ( m_size >= m_pos) {
      throw "out of range";
    } else {
      m_arr[m_pos++] = val;
    }
  }
  int Pop() {
    if (m_pos <= 0) {
      throw "stack empty";
    } return m_arr[--m_pos];
  }
  const void Print() {
    if (m_pos == 0) {
      std::cout << "stack empty";
    } else {
      for (int i = 0; i < m_pos; i++) {
        std::cout << m_arr[i] << " ";
      }
    }
  }
};

class SumStack : public Stack {
private:
  int m_sum;
public:
  SumStack(int size)
    : Stack(size), m_sum(0) {}
  const SumStack& operator=(const SumStack& s) {
    Stack::operator=(s);
    m_sum = s.m_sum;
    return *this;
  }
  void Push(int val) {
    Stack::Push(val);
    m_sum += val;
  }
  int Pop() {
    int val = Stack::Pop();
    m_sum -= val;
    return val;
  }
};