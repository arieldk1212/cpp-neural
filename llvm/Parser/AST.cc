#include "AST.h"
#include "ASTTools.h"

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