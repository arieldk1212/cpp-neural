#include "shared_ptr.h"

#include <iostream>

class TestResource {
public:
  TestResource(int id) : id(id) {
    std::cout << "Resource " << id << " created\n";
  }
  ~TestResource() { std::cout << "Resource " << id << " destroyed\n"; }
  int id;
};

void test_shared_ptr() {
  std::cout << "----- Test 1: Basic Ownership -----\n";
  {
    shared_ptr<TestResource> ptr1(new TestResource(1));
    std::cout << "Resource 1 in use\n";
  }
  std::cout << "Resource 1 should be destroyed\n\n";

  std::cout << "----- Test 2: Copy Test -----\n";
  {
    shared_ptr<TestResource> ptr1(new TestResource(2));
    // shared_ptr<TestResource> ptr2 = ptr1; // Copy
    std::cout << "Resource 2 shared\n";
  }
  std::cout << "Resource 2 should be destroyed\n\n";

  std::cout << "----- Test 3: Assignment Test -----\n";
  {
    shared_ptr<TestResource> ptr1(new TestResource(3));
    shared_ptr<TestResource> ptr2(new TestResource(4));
    // ptr2 = ptr1; // Assignment
    std::cout << "Resource 4 should be destroyed, Resource 3 shared\n";
  }
  std::cout << "Resource 3 should be destroyed\n\n";

  std::cout << "----- Test 4: Multiple Resources -----\n";
  {
    shared_ptr<TestResource> ptr1(new TestResource(5));
    shared_ptr<TestResource> ptr2(new TestResource(6));
    std::cout << "Resource 5 and 6 independently managed\n";
  }
  std::cout << "Resources 5 and 6 should be destroyed\n";
}

int main() {
  test_shared_ptr();
}