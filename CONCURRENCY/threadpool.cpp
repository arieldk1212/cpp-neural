#include "threadpool.h"

int main() {
  std::shared_ptr<ConQueue> Queue = std::make_shared<ConQueue>();
  Producer Prod(Queue);
  Consumer Cons(Queue);

  Cons.RunJob();
}