#ifndef AST_TOOLS_H
#define AST_TOOLS_H

#include "AST.h"
#include <cstdio>

std::unique_ptr<ExprAST> LogError(const char *Str);

std::unique_ptr<ProtorypeExprAST> LogErrorP(const char *Str);

#endif