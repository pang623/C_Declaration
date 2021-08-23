#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

typedef struct Symbol Symbol;
typedef struct SymbolAttrTable SymbolAttrTable;

typedef Symbol *(*NudFuncPtr)(Symbol *symbol);
typedef Symbol *(*LedFuncPtr)(Symbol *symbol, Symbol *left);

struct Symbol {
  int arity;
  int id;
  Token *token;
  Symbol *child[0];
};

struct SymbolAttrTable {
  int prefixRBP;
  int infixRBP;
  int infixLBP;
  NudFuncPtr nud;
  LedFuncPtr led;
};

#endif // SYMBOLTABLE_H
