#include <string>
#include <iostream>

// solves the problem of creating entire product families 
// without specifying their concrete classes.

// Abstract Factory defines an interface for creating all distinct products but leaves the actual product creation to concrete factory classes.
// Each factory type corresponds to a certain product variety.

// The client code calls the creation methods of a factory object instead
// of creating products directly with a constructor call (new operator).
// Since a factory corresponds to a single product variant, all its products will be compatible.

// Client code works with factories and products only through their
// abstract interfaces. This lets the client code work with any product
// variants, created by the factory object.
//You just create a new concrete factory class and pass it to the client code.


/*
It provides an interface for creating families of related or dependent objects without specifying their concrete classes.
It allows clients to create objects of a particular family, without needing to know the actual classes of the objects they are creating.
The term “family of related or dependent objects” refers to a group of objects that are designed to work together or are dependent on each other in some way.
These objects typically share a common theme, purpose, or functionality, and they need to be created in a way that ensures compatibility and consistency among them.
*/

class AbstractProductA {
public:
  virtual ~AbstractProductA() {};
  virtual std::string UsefulFunctionA() const = 0;
};

class ConcreteProductA1 : public AbstractProductA {
public:
  std::string UsefulFunctionA() const override {
    return "the result of the product A1";
  }
};

class ConcreteProductA2 : public AbstractProductA {
public:
  std::string UsefulFunctionA() const override {
    return "the result of the product A2";
  }
};

class AbstractProductB {
public:
  virtual ~AbstractProductB() {};
  virtual std::string UsefulFunctionB() const = 0;
  virtual std::string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product B1.";
  }
  /**
   * The variant, Product B1, is only able to work correctly with the variant,
   * Product A1. Nevertheless, it accepts any instance of AbstractProductA as an
   * argument.
   */
  std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B1 collaborating with ( " + result + " )";
  }
};

class ConcreteProductB2 : public AbstractProductB {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product B2.";
  }
  /**
   * The variant, Product B2, is only able to work correctly with the variant,
   * Product A2. Nevertheless, it accepts any instance of AbstractProductA as an
   * argument.
   */
  std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B2 collaborating with ( " + result + " )";
  }
};

/**
 * The Abstract Factory interface declares a set of methods that return
 * different abstract products. These products are called a family and are
 * related by a high-level theme or concept. Products of one family are usually
 * able to collaborate among themselves. A family of products may have several
 * variants, but the products of one variant are incompatible with products of
 * another.
 */
class AbstractFactory {
 public:
  virtual AbstractProductA *CreateProductA() const = 0;
  virtual AbstractProductB *CreateProductB() const = 0;
};

/**
 * Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible. Note
 * that signatures of the Concrete Factory's methods return an abstract product,
 * while inside the method a concrete product is instantiated.
 */
class ConcreteFactory1 : public AbstractFactory {
 public:
  AbstractProductA *CreateProductA() const override {
    return new ConcreteProductA1();
  }
  AbstractProductB *CreateProductB() const override {
    return new ConcreteProductB1();
  }
};

/**
 * Each Concrete Factory has a corresponding product variant.
 */
class ConcreteFactory2 : public AbstractFactory {
 public:
  AbstractProductA *CreateProductA() const override {
    return new ConcreteProductA2();
  }
  AbstractProductB *CreateProductB() const override {
    return new ConcreteProductB2();
  }
};

void ClientCode(const AbstractFactory &factory) {
  const AbstractProductA *product_a = factory.CreateProductA();
  const AbstractProductB *product_b = factory.CreateProductB();
  std::cout << product_b->UsefulFunctionB() << "\n";
  std::cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
  delete product_a;
  delete product_b;
}

int main() {
  std::cout << "Client: Testing client code with the first factory type:\n";
  ConcreteFactory1 *f1 = new ConcreteFactory1();
  ClientCode(*f1);
  delete f1;
  std::cout << std::endl;
  std::cout << "Client: Testing the same client code with the second factory type:\n";
  ConcreteFactory2 *f2 = new ConcreteFactory2();
  ClientCode(*f2);
  delete f2;
  return 0;
}

// another example

// Abstract product interfaces
 
class Button {
public:
  virtual void render() = 0;
};

// Concrete product classes for light theme
class LightButton : public Button {
public:
    void render() override {
        cout << "Rendering light button" << endl;
    }
};

// Concrete product classes for dark theme
class DarkButton : public Button {
public:
    void render() override {
        cout << "Rendering dark button" << endl;
    }
};

class Checkbox {
public:
    virtual void render() = 0;
};

class LightCheckbox : public Checkbox {
public:
    void render() override {
        cout << "Rendering light checkbox" << endl;
    }
};


class DarkCheckbox : public Checkbox {
public:
    void render() override {
        cout << "Rendering dark checkbox" << endl;
    }
};

// Abstract factory interface
class ThemeFactory {
public:
    virtual Button* createButton() = 0;
    virtual Checkbox* createCheckbox() = 0;
};

// Concrete factory for light theme
class LightThemeFactory : public ThemeFactory {
public:
    Button* createButton() override {
        return new LightButton();
    }

    Checkbox* createCheckbox() override {
        return new LightCheckbox();
    }
};

// Concrete factory for dark theme
class DarkThemeFactory : public ThemeFactory {
public:
    Button* createButton() override {
        return new DarkButton();
    }

    Checkbox* createCheckbox() override {
        return new DarkCheckbox();
    }
};

// Client code
int main() {
    // Create light theme components
    ThemeFactory* lightFactory = new LightThemeFactory();
    Button* lightButton = lightFactory->createButton();
    Checkbox* lightCheckbox = lightFactory->createCheckbox();
    lightButton->render();
    lightCheckbox->render();

    // Create dark theme components
    ThemeFactory* darkFactory = new DarkThemeFactory();
    Button* darkButton = darkFactory->createButton();
    Checkbox* darkCheckbox = darkFactory->createCheckbox();
    darkButton->render();
    darkCheckbox->render();

    // Clean up
    delete lightButton;
    delete lightCheckbox;
    delete darkButton;
    delete darkCheckbox;
    delete lightFactory;
    delete darkFactory;

    return 0;
}