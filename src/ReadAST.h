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
#include "Strings.h"

typedef char *(*ReadFunction)(Symbol *symbol);

#define   readSymbol(symbol)          (readSymbolTable[symbol->id])

char *readAST(Symbol *AST, char *str);
char *arrayRead(Symbol *symbol);
char *ignoreRead(Symbol *symbol);
char *generalRead(Symbol *symbol);
char *identifierRead(Symbol *symbol);
char *pointerRead(Symbol *symbol);
char *functionRead(Symbol *symbol);

#endif // READAST_H
