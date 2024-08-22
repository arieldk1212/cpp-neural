#include <iostream>
#include <algorithm>

class Creature {
private:
  int age, offspring;
  char* name;
public:
  virtual void print_info();
};

class Vampire : public virtual Creature {
private:
  int light_sensitivity;
  Vampire** offspring;
public:
  virtual void print_info() {
    for (int i = 0; i < sizeof(*offspring); i++) {
      std::cout << offspring[i] << "\n";
    }
  }
  double get_light_sensitivity() { return light_sensitivity; }
};

class Werewolf: public virtual Creature {
private:
  int wolf_longtivity, man_longtivity;
  Werewolf** offspring;
};

class VampireWerewolf : public Werewolf, public Vampire {
private:
  Creature** offspring;
};

int pure_vampire_counter(Vampire** creatures, int size) {
  int count = 0;
  for (int i = 0; i < size; i ++) {
    if(typeid(*creatures[i]) == typeid(Vampire)) {
      count++;
    }
  } return count;
}

int vampire_count(Creature** creatures, int size) {
  int count = 0;
  for (int i = 0; i < size; i++) {
    Vampire* v = dynamic_cast<Vampire*>(creatures[i]);
    if (v) {
      count++;
    }
  } return count;
}

int averaged_light_sensitivity(Creature** creatures, int size) {
  double sum = 0;
  int count = 0;
  for (int i = 0; i < size; i++) {
    Vampire* v = dynamic_cast<Vampire*>(creatures[i]);
    if (v) {
      count++;
      sum += v->get_light_sensitivity();
    }
  } return (int)(sum/count);
}