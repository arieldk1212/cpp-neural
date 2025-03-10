#include "AST.h"
#include <memory>

std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<ProtorypeExprAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

#include "Lex.h"

static std::unique_ptr<ExprAST> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(Lex::NumVal);
  Lex::GetNextToken();
  return std::move(Result);
}

/**
 * @brief this function does not constructes a node in the AST.
 */
static std::unique_ptr<ExprAST> ParseParenExpr() {
  Lex::GetNextToken();
  auto V = ParseExpression();
  if (!V) {
    return nullptr;
  }
  if (Lex::CurTok != ')') {
    return LogError("Expected ')");
  }
  Lex::GetNextToken();
  return V;
}

static std::unique_ptr<ExprAST> ParseIndentifierExpr() {
  std::string IdName = Lex::IndentifierStr;

  Lex::GetNextToken();

  if (Lex::CurTok != '(') {
    return std::make_unique<VariableExprAST>(IdName);
  }

  Lex::GetNextToken();
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (Lex::CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression()) {
        Args.push_back(std::move(Arg));
      } else {
        return nullptr;
      }
      if (Lex::CurTok == ')') {
        break;
      }
      if (Lex::CurTok != ',') {
        return LogError("Expected ')' or ',' in argument list");
      }
    }
  }

  Lex::GetNextToken();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

static std::unique_ptr<ExprAST> ParsePrimary() {
  switch (Lex::CurTok) {
  default:
    return LogError("Unknown token when expecting an expression");
  case Lex::tok_indentifier:
    return ParseIndentifierExpr();
  case Lex::tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  }
}

static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS) {
    return nullptr;
  }
  return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
  while (true) {
    int TokPrec = Lex::GetTokPrecedence();
    if (TokPrec < ExprPrec) {
      return LHS;
    }
    int BinOp = Lex::CurTok;
    Lex::GetNextToken();

    auto RHS = ParsePrimary();
    if (!RHS) {
      return nullptr;
    }

    int NextPrec = Lex::GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS) {
        return nullptr;
      }
    }
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

static std::unique_ptr<ProtorypeExprAST> ParsePrototype() {
  if (Lex::CurTok != Lex::tok_indentifier) {
    return LogErrorP("Expected function name in prototype");
  }

  std::string FnName = Lex::IndentifierStr;
  Lex::GetNextToken();

  if (Lex::CurTok != '(') {
    return LogErrorP("Expected '(' in prototype");
  }

  std::vector<std::string> ArgNames;
  while (Lex::GetNextToken() == Lex::tok_indentifier) {
    ArgNames.push_back(Lex::IndentifierStr);
  }
  if (Lex::CurTok != ')') {
    return LogErrorP("Expected ')' in prototype");
  }
  Lex::GetNextToken();

  return std::make_unique<ProtorypeExprAST>(FnName, std::move(ArgNames));
}

static std::unique_ptr<FunctionExprAST> ParseDefinition() {
  Lex::GetNextToken();
  auto Proto = ParsePrototype();
  if (!Proto) {
    return nullptr;
  }

  if (auto E = ParseExpression()) {
    return std::make_unique<FunctionExprAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

static std::unique_ptr<ProtorypeExprAST> ParseExtern() {
  Lex::GetNextToken();
  return ParsePrototype();
}

static std::unique_ptr<FunctionExprAST> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    auto Proto =
        std::make_unique<ProtorypeExprAST>("", std::vector<std::string>());
    return std::make_unique<FunctionExprAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    Lex::GetNextToken();
  }
}

void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern.\n");
  } else {
    Lex::GetNextToken();
  }
}

void HandleTopLevelExpression() {
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr.\n");
  } else {
    Lex::GetNextToken();
  }
}