// proxy!

#include <iostream>
#include <string>

class Subject {
public:
  virtual void Request() const = 0;
};

class RealSubject : public Subject {
public:
  void Request() const override { std::cout << "handling request\n"; }
};

class Proxy : public Subject {
public:
  Proxy(RealSubject *real_subject)
      : m_RealSubject(new RealSubject(*real_subject)) {}
  ~Proxy() { delete m_RealSubject; }

  void Request() const override {
    if (this->CheckAccess()) {
      this->m_RealSubject->Request();
      this->LogAccess();
    }
  }

private:
  bool CheckAccess() const {
    // logic
    return true;
  }
  void LogAccess() const { std::cout << "Log"; }

private:
  RealSubject *m_RealSubject;
};

void ClientCode(const Subject &subject) { subject.Request(); }

int main() {
  // with out proxy
  RealSubject *real = new RealSubject;
  ClientCode(*real);
  // with proxy
  Proxy *proxy = new Proxy(real);
  ClientCode(*proxy);
}