#ifndef LEX_H
#define LEX_H

#include "ASTTools.h"
#include <_ctype.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
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

static std::map<char, int> BinopPrecedence;
static int GetTokPrecedence() {
  if (!isascii(CurTok)) {
    return -1;
  }
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0) {
    return -1;
  }
  return TokPrec;
  // BinopPrecedence['<] = 10; i.e. (defined in Main).
}

}; // namespace Lex

/* Parsing Methods */

static std::unique_ptr<ExprAST> ParseNumberExpr();
static std::unique_ptr<ExprAST> ParseParenExpr();
static std::unique_ptr<ExprAST> ParseIndentifierExpr();
static std::unique_ptr<ExprAST> ParsePrimary();
static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS);
static std::unique_ptr<ProtorypeExprAST> ParsePrototype();
static std::unique_ptr<FunctionExprAST> ParseDefinition();
static std::unique_ptr<ProtorypeExprAST> ParseExtern();
static std::unique_ptr<FunctionExprAST> ParseTopLevelExpr();

/* Main Loop Definition */

static void HandleDefinition();

static void HandleExtern();

static void HandleTopLevelExpression();

static void MainLoop() {
  while (true) {

    fprintf(stderr, "ready> ");
    switch (Lex::CurTok) {
    case Lex::tok_eof:
      return;
    case ';':
      Lex::GetNextToken();
      break;
    case Lex::tok_def:
      HandleDefinition();
      break;
    case Lex::tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

#endif