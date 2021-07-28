#ifndef C_DECLARATION_H
#define C_DECLARATION_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "Arity.h"

//Macros
#define   getInfixLBP(symbol)         (symbolTable[symbol->id].infixLBP)
#define   getInfixRBP(symbol)         (symbolTable[symbol->id].infixRBP)
#define   getPrefixRBP(symbol)        (symbolTable[symbol->id].prefixRBP)

#define   nudOf(symbol)               (symbolTable[symbol->id].nud(symbol))
#define   ledOf(symbol, left)         (symbolTable[symbol->id].led(symbol, left))

typedef struct SymbolAttrTable SymbolAttrTable;
typedef Symbol *(*NudFuncPtr)(Symbol *symbol);
typedef Symbol *(*LedFuncPtr)(Symbol *symbol, Symbol *left);

struct SymbolAttrTable {
  int prefixRBP;
  int infixRBP;
  int infixLBP;
  NudFuncPtr nud;
  LedFuncPtr led;
};

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

#endif // C_DECLARATION_H
