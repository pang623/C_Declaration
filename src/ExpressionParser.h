#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Exception.h"
#include "CDecl_Errors.h"
#include "Arity.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "Tdop.h"

#define   NIL   1000

extern SymbolParser *symbolParser;

Symbol *expression(int rbp);
Symbol *prefixNud(Symbol *symbol);
Symbol *infixLedL(Symbol *symbol, Symbol *left);
Symbol *infixLedR(Symbol *symbol, Symbol *left);
Symbol *suffixLed(Symbol *symbol, Symbol *left);
Symbol *identityNud(Symbol *symbol);
Symbol *errorNud(Symbol *symbol);
Symbol *missingOperandNud(Symbol *symbol);
Symbol *errorLed(Symbol *symbol, Symbol *left);
Symbol *identityLed(Symbol *symbol, Symbol *left);
Symbol *parentNud(Symbol *symbol);
Symbol *sqrBracketLed(Symbol *symbol, Symbol *left);

#endif // EXPRESSIONPARSER_H
