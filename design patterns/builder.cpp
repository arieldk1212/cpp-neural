#include <cstddef>
#include <iostream>

// building complex things in little steps, abstraction of complex.

class Product1 {
public:
  std::vector<std::string> parts_;
  void list_parts_() const { std::cout << "product parts";
  for (size_t i = 0; i < parts_.size(); i ++) {
    if (parts_[i] == parts_.back()) {
      std::cout << parts_[i];
    } else { std::cout << parts_[i] << " , "; }

    }
  }
};

class Builder {
public:
  virtual ~Builder();
  virtual void ProductPartA() const = 0;
  virtual void ProductPartB() const = 0;
  virtual void ProductPartC() const = 0;
};

class ConcreteBuilder1 : public Builder {
public:
  ConcreteBuilder1() {
    this->Reset();
  }
  ~ConcreteBuilder1() { delete product; }
  void Reset() {
    this->product = new Product1();
  }
  void ProducePartA() const override {
    this->product->parts_.push_back("PartA1");
  }
  void ProducePartC() const override {
    this->product->parts_.push_back("PartC1");
  }
  void ProducePartB() const override {
    this->product->parts_.push_back("PartB1");
  }
  Product1* get_product() {
    Product1* result = this->product;
    this->Reset();
    return result;
  }
private:
  Product1* product;
};


/**
 * The Director is only responsible for executing the building steps in a
 * particular sequence. It is helpful when producing products according to a
 * specific order or configuration. Strictly speaking, the Director class is
 * optional, since the client can control builders directly.
 */

class Director {
public:
  void set_builder(Builder* builder) {
    this->builder = builder;
  }
  void BuildMinimalViableProduct() {
    this->builder->ProductPartA();
  }
  void BuildFullFeaturedProduct() {
    this->builder->ProductPartA();
    this->builder->ProductPartB();
    this->builder->ProductPartC();
  }
private:
  Builder* builder;
};

/**
 * The client code creates a builder object, passes it to the director and then
 * initiates the construction process. The end result is retrieved from the
 * builder object.
 */
/**
 * I used raw pointers for simplicity however you may prefer to use smart
 * pointers here
 */

 void ClientCode(Director& director)
{
  ConcreteBuilder1* builder = new ConcreteBuilder1();
  director.set_builder(builder);
  std::cout << "Standard basic product:\n"; 
  director.BuildMinimalViableProduct();
  
  Product1* p= builder->get_product();
  p->list_parts_();
  delete p;

  std::cout << "Standard full featured product:\n"; 
  director.BuildFullFeaturedProduct();

  p= builder->get_product();
  p->list_parts_();
  delete p;

  // Remember, the Builder pattern can be used without a Director class.
  std::cout << "Custom product:\n";
  builder->ProducePartA();
  builder->ProducePartC();
  p=builder->get_product();
  p->list_parts_();
  delete p;

  delete builder;
}

int main(){
    Director* director= new Director();
    ClientCode(*director);
    delete director;
    return 0;    
}
