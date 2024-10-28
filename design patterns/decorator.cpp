// structural design that lets you attach a new behaviors to objects by placing these objects
// inside special wrapper objects that contain the behaviors.

#include <string>
#include <iostream>

class Component {
public:
  virtual ~Component() {}
  virtual std::string Operation() const = 0;
};

/* the default implementations of the operations */
class ConcreteComponent : public Component {
public:
  std::string Operation() const override { return "ConcreteComponent"; }
};

class Decorator : public Component {
public:
  Decorator(Component* component) : component_(component) {}
  std::string Operation() const override { return this->component_->Operation(); }
protected:
  Component* component_;
};

class ConcreteDecoratorA : public Decorator {
public:
  ConcreteDecoratorA(Component* component) : Decorator(component) {}
  std::string Operation() const override {
    return "COncreteDecoratorA(" + Decorator::Operation() + ")";
  }
};

class ConcreteDecoratorB : public Decorator {
public:
  ConcreteDecoratorB(Component* component) : Decorator(component) {}
  std::string Operation() const override {
    return "COncreteDecoratorB(" + Decorator::Operation() + ")";
  }
};

// the client code works with all the objects using the Component interface,
// this way it can stay independent of the concrete classes of components it works with.
void ClientCode(Component* component) {
  std::cout << "RESULT: " << component->Operation();
}

int main () {
  // the client code can support both simple components.
  Component* simple = new ConcreteComponent;
  std::cout << "Client: a simple component:\n";
  ClientCode(simple);
  std::cout << "\n";

  // as well as the decorated ones.
  Component* decorator1 = new ConcreteDecoratorA(simple);
  Component* decorator2 = new ConcreteDecoratorB(decorator1);
  std::cout << "Client : a decorated component\n";
  ClientCode(decorator2);

  delete simple;
  delete decorator1;
  delete decorator2;
  return 0;
}
