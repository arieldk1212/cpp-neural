#include "threadpool.h"

int main() {
  TPool threadpool;

  for (int i = 0; i < 10; ++i) {
    threadpool.Enque();
  }
}