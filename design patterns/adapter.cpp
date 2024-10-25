// adapter is a structural design [attern that allows objects with incompatible interfacs to collaborate.

#include <iostream>
#include <string>

class Target {
public:
  virtual ~Target();
  virtual std::string Request() const {
    return "Target: the default target's behavior";
  }
};

class Adaptee {
public:
  std::string specific_request() const {
    return "asdasdasd";
  }
};

class Adapter : public Target {
public:
  Adapter(Adaptee* adaptee) : adaptee_(adaptee){}
  std::string Request() const override {
    std::string to_reverse = this->adaptee_->specific_request();
    std::reverse(to_reverse.begin(), to_reverse.end());
    return "Adapter: (TRANSLATED) " + to_reverse;
  }
private:
  Adaptee* adaptee_;
};

void ClientCode(const Target* target) {
  std::cout << target->Request();
}

int main () {
  std::cout << "Client: I can work just fine with the Target objects:\n";
  Target *target = new Target;
  ClientCode(target);
  std::cout << "\n\n";
  Adaptee *adaptee = new Adaptee;
  std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
  std::cout << "Adaptee: " << adaptee->specific_request();
  std::cout << "\n\n";
  std::cout << "Client: But I can work with it via the Adapter:\n";
  Adapter *adapter = new Adapter(adaptee);
  ClientCode(adapter);
  std::cout << "\n";

  delete target;
  delete adaptee;
  delete adapter;
  return 0;
}
