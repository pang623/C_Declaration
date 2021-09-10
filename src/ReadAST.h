#ifndef READAST_H
#define READAST_H

#include "Exception.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "CDecl.h"
#include "ExpressionParser.h"
#include "Tdop.h"
#include "CDecl_Errors.h"
#include "Arithmetic.h"

typedef char *(*ReadFunction)(Symbol *symbol);
typedef int (*EvaluateFunction)(int, int);

#define   getSymbolInteger(symbol)    (((IntegerToken *)(*symbol).token)->value)
#define   readSymbol(symbol)          (readSymbolTable[symbol->id])
#define   evaluate(symbol)            (evaluateSymbolTable[symbol->id])

char *readAST(Symbol *AST, char *str);
char *concat(char *s1, char *s2);
char *arrayRead(Symbol *symbol);
char *ignoreRead(Symbol *symbol);
char *generalRead(Symbol *symbol);
char *identifierRead(Symbol *symbol);
char *pointerRead(Symbol *symbol);
char *functionRead(Symbol *symbol);

char *convertIntToStr(int num);
int isExpressionReducible(Symbol *symbol);
int integerEvaluate(Symbol *expression);

#endif // READAST_H
