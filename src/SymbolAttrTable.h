#ifndef SYMBOLATTRTABLE_H
#define SYMBOLATTRTABLE_H

#include "Token.h"
#include "Tokenizer.h"

typedef struct Symbol Symbol;
typedef struct SymbolAttrTable SymbolAttrTable;

typedef Symbol *(*NudFuncPtr)(Symbol *symbol);
typedef Symbol *(*LedFuncPtr)(Symbol *symbol, Symbol *left);

struct Symbol {
  int arity;
  int id;
  Token *token;
  Symbol *child[2];
};

struct SymbolAttrTable {
  int prefixRBP;
  int infixRBP;
  int infixLBP;
  NudFuncPtr nud;
  LedFuncPtr led;
};

#endif // SYMBOLATTRTABLE_H
