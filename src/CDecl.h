#ifndef CDECL_H
#define CDECL_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "Arity.h"

#define   NIL   1000

//Macros
#define   getInfixLBP(symbol)         (symbolTable[symbol->id].infixLBP)
#define   getInfixRBP(symbol)         (symbolTable[symbol->id].infixRBP)
#define   getPrefixRBP(symbol)        (symbolTable[symbol->id].prefixRBP)

#define   nudOf(symbol)               (symbolTable[(symbol)->id]).nud
#define   ledOf(symbol)               (symbolTable[(symbol)->id]).led

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

#endif // CDECL_H
