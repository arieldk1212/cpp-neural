#include "Parser/Lex.h"
#include <iostream>


int main() {
  Lex::BinopPrecedence['<'] = 10;
  Lex::BinopPrecedence['+'] = 20;
  Lex::BinopPrecedence['-'] = 30;
  Lex::BinopPrecedence['*'] = 40;

  fprintf(stderr, "ready> ");
  Lex::GetNextToken();

  MainLoop();

  return 0;
}