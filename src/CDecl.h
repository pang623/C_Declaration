#ifndef CDECL_H
#define CDECL_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "ExpressionParser.h"
#include "Arity.h"
#include "SymbolAttrTable.h"
#include "Tdop.h"

extern SymbolParser *symbolParser;

Symbol *groupingNud(Symbol *symbol, Symbol *left);
Symbol *funcLed(Symbol *symbol, Symbol *left);
Symbol *pointerNud(Symbol *symbol, Symbol *left);
Symbol *pointerLed(Symbol *symbol, Symbol *left);
Symbol *combineAST(Symbol *AST, Symbol *oldAST);
Symbol *cDecl(int rbp);

#endif // CDECL_H
