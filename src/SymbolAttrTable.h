#ifndef SYMBOLATTRTABLE_H
#define SYMBOLATTRTABLE_H

#include "Token.h"
#include "Tokenizer.h"
#include "Symbol_Id.h"
#include "Symbol.h"

typedef struct SymbolAttrTable SymbolAttrTable;

typedef Symbol *(*NudFuncPtr)(Symbol *symbol, Symbol *left);
typedef Symbol *(*LedFuncPtr)(Symbol *symbol, Symbol *left);

struct SymbolAttrTable {
  int prefixRBP;
  int infixRBP;
  int infixLBP;
  NudFuncPtr nud;
  LedFuncPtr led;
};

#endif // SYMBOLATTRTABLE_H
