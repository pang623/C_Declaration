#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Exception.h"
#include "CDecl_Errors.h"
#include "Arity.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "Tdop.h"
#include "Arithmetic.h"

#define   NIL   1000

typedef int (*EvaluateFunction)(int, int);

#define   getSymbolInteger(symbol)    (((IntegerToken *)(*symbol).token)->value)
#define   evaluate(symbol)            (evaluateSymbolTable[symbol->id])

extern SymbolParser *symbolParser;

int isExpressionReducible(Symbol *symbol);
int isExpressionHasFloatNum(Symbol *symbol);
int integerEvaluate(Symbol *expression);
Symbol *expression(int rbp);
Symbol *prefixNud(Symbol *symbol, Symbol *left);
Symbol *infixLedL(Symbol *symbol, Symbol *left);
Symbol *infixAssgnLed(Symbol *symbol, Symbol *left);
Symbol *prefixIncDecNud(Symbol *symbol, Symbol *left);
Symbol *suffixIncDecLed(Symbol *symbol, Symbol *left);
Symbol *identityNud(Symbol *symbol, Symbol *left);
Symbol *errorNud(Symbol *symbol, Symbol *left);
Symbol *missingOperandNud(Symbol *symbol, Symbol *left);
Symbol *errorLed(Symbol *symbol, Symbol *left);
Symbol *identityLed(Symbol *symbol, Symbol *left);
Symbol *parentNud(Symbol *symbol, Symbol *left);
Symbol *sqrBracketLed(Symbol *symbol, Symbol *left);

#endif // EXPRESSIONPARSER_H
