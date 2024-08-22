// a public member is accessible -
// within its class
// within the inheritance hierrachy
// from an object of the class

// a private member is only accessible within its class
// not within the inheritance hierarchy
// not from an object of the class

// what if we want ot deny access form an object, but allow it in the class hierarchy? meaning in inheritance it will be accessiable.

// a protected member is the way to do it
// accessible within its class
// within the inheritance hierarchy
// not from an object of the class.

class A {
public:
  int public_member;
protected:
  int protected_member;
private:
  int private_member;
};

class B : protected A {
  void foo() {
    public_member = 5; // good
    protected_member = 5; // good due to adding proteced in inheritance
    // private_member = 5; // private!
  }
};

class C : public B {
  void foo() {
    public_member = 6; // its protected
    protected_member = 6; // its protectd
    // private_member = 4; // private!
    // but when we change the class B to class B : private A, all the fields in class c are inaccessiable due to being not protected
    // but private!
  }
};

// the final permission will allways eb the more restricting permission.
// when the base class does not implement enough protection,
// we can fix it using protected / private inheritance.

// we can implement inheritance by doing
// Class Duck : public Flyer, public Swimmer

// in inheritance we can create derived classes (lets say for this example 2) that inherit from the SAME base class,
// which is good somethimes, why? lets say is a clock - digital and analog, the both are different from are derived from the same
// class which is clock, and its good because we can set different times in each class, but if we switch the clock type and have it 
// about guitars, base guitar and electric guitar, guitar is a guitar, and in the memory is will show up TWICE, we dont need it...
// so we use the same base class for the 2 different types of guitars.
// the way fix the problem that guitar class gives us and NOT clock class, is by using the virtual keyword before public.
// like so:

class Guitar {};
class ElectricGuitar : virtual public Guitar {};
class BaseGuitar : virtual public Guitar {};
class ElectricBaseGuitar : public ElectricGuitar, public BaseGuitar {};

// then if we do
// ElectricGuitar eg;
// BaseGuitar bg;
// ElectricBaseGuitar ebg; -> it points to Guitar Class once! becasue the upper two classes are virtual.

// use virtual inheritance ONLY when needed.

// CTOR PROBLEM..
// when dealing with base class, two derived classes that uses virtual inheritance, and derived derived class the derives from the two
// upper classes, to write correctly the CTOR for the base class we do it like so:
// C()
//  : A(0) {};
// so we dont create couple CTOR's