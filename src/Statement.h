#ifndef STATEMENT_H
#define STATEMENT_H

#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "CDecl.h"
#include "ExpressionParser.h"
#include "KeywordType.h"

extern SymbolParser *symbolParser;

#define   fudOf(type)                (keywordTable[type])

typedef Symbol *(*FudFuncPtr)(int rbp);

Symbol *statement();
Symbol *forFud(int rbp);
Symbol *whileFud(int rbp);
Symbol *ifFud(int rbp);
Symbol *caseFud(int rbp);
Symbol *continueFud(int rbp);
Symbol *breakFud(int rbp);

#endif // STATEMENT_H
