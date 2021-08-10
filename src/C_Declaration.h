#ifndef C_DECLARATION_H
#define C_DECLARATION_H

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
  int idType[2];
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
Symbol *parentNud(Symbol *symbol);
void verifyExpressionFullyParsed(Tokenizer *tokenizer);
Symbol *parse(int rbp);

#endif // C_DECLARATION_H
