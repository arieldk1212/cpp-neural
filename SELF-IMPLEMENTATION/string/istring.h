#ifndef ISTRING_H
#define ISTRING_H

#include <string>


struct NonHeapString {

};

struct HeapString {
  
};

class IString {
public:
  IString(const char *data) {}
  ~IString() {}
private:

};

#endif