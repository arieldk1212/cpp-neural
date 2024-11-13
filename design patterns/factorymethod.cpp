// Creatinal Design, Provides an interface for creating objects in the 
// a superclass, but allows subclass to alter the type of objects that
// will be created.


/*
The factory method pattern is a design pattern used in object-oriented programming to create objects without specifying the exact class of the object that will be created.
It provides an interface for creating objects in a superclass but allows subclasses to alter the type of objects that will be created.
You need pointer or reference to a base class, and you also need a virtual factory method in the base class in order to call the correct instance methods of the object created by the factory method. This is Polymorphism.
The reason you need a pointer or a reference to a base class is because an object of derived class should be able to replace an object of a parent class.
In other words, wherever an object of a parent (base) class is expected, you can use an object of its derived class(es).
This is substitutability principle of inheritance (also known by Liskov Substitution principle). This allows for flexibility in usage.

The problem isn’t necessarily with using if-else conditions per se. Rather, the issue arises from the need to understand how to create objects of the desired class without the assistance of the Factory Method pattern.
Without this pattern, if an object needs to be created at multiple points in your code, you would be required to repeat the same instantiation logic each time.
Furthermore, if the logic for object creation were to change (for example, if additional constructor arguments were required), you would need to update every instance where the object is instantiated.

When it is useful?
When you have a class that can have multiple subclasses, each providing a different implementation.
When you want to decouple the client code from the concrete classes being instantiated.
When you need to extend the application by adding new types of objects without modifying existing code.

*/
// AbstractProduct
class Logger {
public:
    virtual void log(const string& message) = 0;
};

// ConcreteProductA
class FileLogger : public Logger {
public:
    void log(const string& message) {
        // Code to log message to a file
    }
};

// ConcreteProductB
class ConsoleLogger : public Logger {
public:
    void log(const string& message) {
        // Code to log message to the console
    }
};
class LoggerFactory {
public:
    virtual Logger* createLogger() = 0;
};

// ProductFactoryA
class FileLoggerFactory : public LoggerFactory {
public:
    Logger* createLogger() {
        return new FileLogger();
    }
};

// ProductFactoryB
class ConsoleLoggerFactory : public LoggerFactory {
public:
    Logger* createLogger() {
        return new ConsoleLogger();
    }
};

int main() {
    LoggerFactory* factory = new FileLoggerFactory();
    Logger* logger = factory->createLogger();
    logger->log("This is a file log");

    delete logger;
    delete factory;

    factory = new ConsoleLoggerFactory();
    logger = factory->createLogger();
    logger->log("This is a console log");

    delete logger;
    delete factory;
    return 0;
}


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