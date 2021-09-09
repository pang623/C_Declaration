#ifndef CDECL_H
#define CDECL_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "ExpressionParser.h"
#include "Arity.h"
#include "SymbolAttrTable.h"
#include "Tdop.h"
#include "KeywordType.h"

#define   fudOf(type)                (keywordTable[type])

typedef Symbol *(*FudFuncPtr)(int rbp);

extern SymbolParser *symbolParser;

Symbol *statement();
Symbol *cDecl(int rbp);
Symbol *forFud(int rbp);
Symbol *groupingNud(Symbol *symbol, Symbol *left);
Symbol *funcLed(Symbol *symbol, Symbol *left);
Symbol *pointerNud(Symbol *symbol, Symbol *left);
Symbol *pointerLed(Symbol *symbol, Symbol *left);
Symbol *combineAST(Symbol *AST, Symbol *oldAST);
Symbol *forFud(int rbp);
Symbol *whileFud(int rbp);
Symbol *ifFud(int rbp);
Symbol *caseFud(int rbp);

#endif // CDECL_H
