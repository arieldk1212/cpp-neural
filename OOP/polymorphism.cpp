// constexpr executes from the compiler each time, meaning we can make it faster or a certain function faster by indicating this key word.

// in c++ binding (associating the memory address of functions/variables) is static unless instructed otherwise.
// meaning that we need to use virtual.

// class Person {
// public:
//   void walk();
//   virtual void talk();
// };

// class Employee : public Person {
// public:
//   virtual void talk();
//   void work();
// };

// class Developer : public Employee{
// public:
//   virtual void talk();
//   void work();
// };

// int main() {
//   Employee e;
//   e.walk(); // executes from Person
//   e.talk(); // executes from Employee
//   e.work(); // executes from Employee

//   Person* p = new Employee;
//   p->walk(); // executes from Person
//   p->talk(); // executes from Person -> now after adding virtual to talk in Person class, it will be executed from empolyee!
//   // p->work(); // undefined, not from Person.
//   delete p;

//   Person* p = new Developer;
//   p->talk(); // executes from Developer
//   p->walk(); // executes from Person
//   // p->work(); // undefined, not from Person.
//   // after this line, i added virtual to all talk functions across all objects.
// }


// GENERAL CONTAINER

class Shape {
private:
  int color;
public:
  virtual void draw();
};

class Triangle : public Shape {
private:
  int a, b, c;
  virtual void draw();
};

class Circle : public Shape {
private:
  int center;
  double radius;
  virtual void draw();
};

// int main() {
//   Shape** shapes = new Shape*[2];
//   shapes[1] = new Triangle();
//   shapes[2] = new Circle();

//   for (int i = 0; i < 2; i++) {
//     shapes[i]->draw();
//   }

//   return 0;
// }


// GENERIC ALGORITHM

#include <algorithm>

class Worker {
private:
  int age, salary;
  char* name;
public:
  const Worker& operator>(const Worker& w);
};

const Worker& Worker::operator>(const Worker& w) {
  return w;
}

class WorkerSorter {
public:
  void sort(Worker** workers, int size);
};

void WorkerSorter::sort(Worker** workers, int size) {
  bool swapped;
  do {
    swapped = false;
    for (int i = 1; i < size; i++) {
      if (*workers[i-1] > *workers[i]) { // by value, without specifiying the > function, it doesnt know how to check who is bigger
      // by what value? so we need to create it for every field.. age, salary, make it generic in a non-generic way, decide at runtime.
        std::swap(workers[i-1], workers[i]); // by address
        swapped = true;
      }
    }
  } while (swapped);
}


// ABSTRACT CLASSES
 
// example of a simple abstract class

class AbstractShape {
private:
  int color;
public:
  virtual void draw() = 0; // in this case, we simply create the function, set it to 0, making iti abstract, PURE METHOD.
};

//////////////////////////////////

class Person {
public:
  void walk();
  virtual void talk();
};

class Employee : public Person {
public:
  virtual void work();
  virtual void talk();
};

class Developer : public Employee {
public:
  virtual void talk();
  virtual void work();
};

int main() { 
  Person* p = new Employee;
  // typeid(p).name() -> Person*
  // typeid(*p).name() -> Employee
  // typeid(p) == typeid(Employee*) -> false
  // typeid(*p) == typeid(Employee) -> true, meaning Employee* e = (Employee*)p;
  Person* p = new Developer;
  // typeid(*p) == typeid(Employee) -> false!, solution for this is -> Employee* e = dynamic_cast<Employee*>(p);, static_cast 
  // is for going from derived class to base class(upcast), dynamic_cast is for base class to derived class(downcast)
  // further explanation -> Person* p, Employee* p -> dynamic_cast<Employee*>(p) asks: can you look at the runtime object
  // as an Employee? in other words, does p points to an object which is a type of Employee?
}