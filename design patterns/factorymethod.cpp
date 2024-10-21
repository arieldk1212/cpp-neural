// Creatinal Design, Provides an interface for creating objects in the 
// a superclass, but allows subclass to alter the type of objects that
// will be created.

#include <string>
#include <iostream>

// interface that declares the operations that all concrete products must implement.
class Product {
public:
  virtual ~Product() {};
  virtual std::string Operation() const = 0;
};

// concrete products provde various  implementations of the product interface.
class ConcreteProduct1 : public Product {
public:
  std::string Operation() const override {
    return "{result of the ConcreteProduct1}";
  }
};

class ConcreteProduct2 : public Product {
public:
  std::string Operation() const override {
    return "{result of the ConcreteProduct2}";
  }
};

// The creator class declares the factory method that is supposed to return
// an object of a product class. the creators subclasses usually provide
// the implementation of this method.
class Creator {
public:
  virtual ~Creator() {};
  virtual Product* FactoryMethod() const = 0;
  std::string SomeOperation() const  {
    Product* product = this->FactoryMethod();
    std::string result = "creator: the sames creators code has just worked with " + product->Operation();
    delete product;
    return result;
  }
};

// concrete creators override the factory method in order to change the
// RESULTING products type
class ConcreteCreator1 : public Creator {
public:
  Product* FactoryMethod() const override {
    return new ConcreteProduct1();
  }
};

class ConcreteCreator2 : public Creator {
public:
  Product* FactoryMethod() const override {
    return new ConcreteProduct2();
  }
};

// the client code works with an instance of a concrete creator, 
// albeit through its base interface, as long as the client keeps working
// with the creator via the base interface, you can pass it any creators subclass.

void ClientCode(const Creator& creator) {
  std::cout << "client: im not aware of my father, but it works\n"
      << creator.SomeOperation() << std::endl;
}

int main() {
  Creator* creator = new ConcreteCreator1();
  ClientCode(*creator);
  Creator* creator2 = new ConcreteCreator2();
  ClientCode(*creator);
  delete creator;
  delete creator2;
}