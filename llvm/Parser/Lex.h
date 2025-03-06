#ifndef LEX_H
#define LEX_H

#include "AST.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace Lex {

enum Token {
  tok_eof = -1,

  /* Commands */
  tok_def = -2,
  tok_extern = -3,

  /* Primary */
  tok_indentifier = -4,
  tok_number = -5
};

inline static std::string IndentifierStr; /* Case: Token is -4 */
inline static double NumVal;              /* Case: Token is -5 */

static int GetTok() {
  static int LastChar = ' ';

  while (std::isspace(LastChar)) {
    LastChar = std::getchar();
  }
  if (std::isalpha(LastChar)) {
    IndentifierStr = LastChar;
    while (std::isalnum(LastChar = std::getchar())) {
      IndentifierStr += LastChar;
    }
    if (IndentifierStr == "def") {
      return Token::tok_def;
    }
    if (IndentifierStr == "extern") {
      return Token::tok_extern;
    }
    return tok_indentifier;
  }
  if (std::isdigit(LastChar) || LastChar == '.') {
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = std::getchar();
    } while (std::isdigit(LastChar) || LastChar == '.');
    NumVal = std::strtod(NumStr.c_str(), 0);
    return Token::tok_number;
  }
  if (LastChar == '#') {
    do {
      LastChar = std::getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    if (LastChar != EOF) {
      return GetTok();
    }
  }
  if (LastChar == EOF) {
    return Token::tok_eof;
  }
  int ThisChar = LastChar;
  LastChar = std::getchar();
  return ThisChar;
}

static int CurTok;
static int GetNextToken() { return CurTok = GetTok(); }

}; // namespace Lex

/* Parsing Methods */

static std::unique_ptr<ExprAST> ParseNumberExpr();
static std::unique_ptr<ExprAST> ParseParenExpr();
static std::unique_ptr<ExprAST> ParseIndentifierExpr();
static std::unique_ptr<ExprAST> ParsePrimary();

#endif