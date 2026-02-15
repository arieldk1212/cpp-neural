#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

class ExprAST {
public:
  virtual ~ExprAST() = default;
};

/* AST For Numerical Literals */
class NumberExprAST : public ExprAST {
public:
  NumberExprAST(double Val) : m_Val(Val) {}

private:
  double m_Val;
};

/* AST For Variables "a" */
class VariableExprAST : public ExprAST {
public:
  VariableExprAST(const std::string &Name) : m_Name(Name) {}

private:
  std::string m_Name;
};

/* AST For Binary Operations */
class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : m_Op(Op), m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}

private:
  char m_Op;
  std::unique_ptr<ExprAST> m_LHS, m_RHS;
};

/* AST For Function Calls */
class CallExprAST : public ExprAST {
public:
  CallExprAST(const std::string &Calle,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : m_Calle(Calle), m_Args(std::move(Args)) {}

private:
  std::string m_Calle;
  std::vector<std::unique_ptr<ExprAST>> m_Args;
};

/* AST For Function's Prototype, Capturing Name, Arguments */
class ProtorypeExprAST {
public:
  ProtorypeExprAST(const std::string &Name, std::vector<std::string> Args)
      : m_Name(Name), m_Args(std::move(Args)) {}

  const std::string &GetName() const { return m_Name; }

private:
  std::string m_Name;
  std::vector<std::string> m_Args;
};

/* AST For Function Definitions Itself */
class FunctionExprAST : public ExprAST {
public:
  FunctionExprAST(std::unique_ptr<ProtorypeExprAST> Proto,
                  std::unique_ptr<ExprAST> Body)
      : m_Proto(std::move(Proto)), m_Body(std::move(Body)) {}

private:
  std::unique_ptr<ProtorypeExprAST> m_Proto;
  std::unique_ptr<ExprAST> m_Body;
};

#endif