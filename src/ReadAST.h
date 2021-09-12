#ifndef READAST_H
#define READAST_H

#include "Exception.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "CDecl.h"
#include "ExpressionParser.h"
#include "Tdop.h"
#include "CDecl_Errors.h"
#include "Statement.h"

typedef char *(*ReadFunction)(Symbol *symbol);

#define   readSymbol(symbol)          (readSymbolTable[symbol->id])

char *readAST(Symbol *AST, char *str);
char *concat(char *s1, char *s2);
char *arrayRead(Symbol *symbol);
char *ignoreRead(Symbol *symbol);
char *generalRead(Symbol *symbol);
char *identifierRead(Symbol *symbol);
char *pointerRead(Symbol *symbol);
char *functionRead(Symbol *symbol);
char *convertIntToStr(int num);

#endif // READAST_H
