// structural design pattern that lets you compose objecs into tree structures and then work with these structures as if they were individual objects.
// makes only sense if the core model of your app can be represented as a tree.

#include <string>
#include <list>

class Component {
protected:
  Component* parent_;
public:
  virtual ~Component();
  void set_parent(Component* parent) { this->parent_ = parent; }
  Component* get_parent() const { return this->parent_; }
  virtual void add(Component* component) {}
  virtual void remove(Component* component) {}
  virtual bool is_composite() const { return false; }
  virtual std::string operation() const = 0;
};

// object that cant have any children
class Leaf : public Component {
public:
  std::string operation() const override {
    return "Leaf";
  }
};

// object that can have children
class Composite : public Component {
protected:
  std::list<Component* > children_;
public:
  void add(Component* component) override {
    this-> children_.push_back(component);
    component->set_parent(this);
  }
  void remove(Component* component) override {
    children_.remove(component);
    component->set_parent(nullptr);
  }
  bool is_composite() const override {
    return true;
  }
  std::string operation() const override {
    std::string result;
    for (const Component* c : children_) {
      if (c == children_.back()) {
        result += c->operation();
      } else { result += c->operation() + "+"; }
    }
    return "Branch(" + result + ")";
  }
};
