#ifndef READAST_H
#define READAST_H

#include "Symbol.h"

typedef char *(*ReadFunction)(Symbol *symbol);

#define   readSymbol(symbol)          (readSymbolTable[symbol->id])

char *readAST(Symbol *AST, char *str);
char *concat(char *s1, char *s2);
char *translate(char *cDecl);
char *arrayRead(Symbol *symbol);
char *ignoreRead(Symbol *symbol);
char *generalRead(Symbol *symbol);
char *identifierRead(Symbol *symbol);
char *pointerRead(Symbol *symbol);
char *functionRead(Symbol *symbol);

#endif // READAST_H
