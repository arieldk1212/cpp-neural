// a bridge is a structural design pattern that lets you split
// a large class or a set of closely related classes into twp
// separate hierarchies-abstraction and implementation which can be 
// developed independently of each other.
// Abstraction <-> Implementation
// GUI of the app <-> System's API's

#include <string>
#include <iostream>

class Implementation {
public:
  virtual ~Implementation() {}
  virtual std::string OperationImplementation() const = 0;
};

class ConcreteImplementationA : public Implementation {
public:
  std::string OperationImplementation() const override {
    return "the result on platform A";
  }
};

class ConcreteImplementationB : public Implementation {
public:
  std::string OperationImplementation() const override {
    return "the result on platform B";
  } 
};

class Abstraction {
public:
  Abstraction(Implementation* implementation)
    :  implementation_(implementation) {}
  ~Abstraction() {}
  virtual std::string Operation() const {
    return "base operation with: " + this->implementation_->OperationImplementation();
  }
protected:
  Implementation* implementation_;
};

class ExtentedAbstraction : public Abstraction {
public:
  ExtentedAbstraction(Implementation* implementation)
    : Abstraction(implementation) {}
  std::string Operation() const override {
    return "extended operation with: " + this->implementation_->OperationImplementation();
  }
};

void ClientCode(const Abstraction& abstraction) {
  std::cout << abstraction.Operation();
}

int main() {
  Implementation* implementation = new ConcreteImplementationA;
  Abstraction* abstraction = new Abstraction(implementation);
  ClientCode(*abstraction);

  implementation = new ConcreteImplementationB;
  abstraction = new ExtentedAbstraction(implementation);

  delete implementation;
  delete abstraction;

  return 0;
}