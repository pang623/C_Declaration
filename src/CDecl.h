#ifndef CDECL_H
#define CDECL_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "ExpressionParser.h"
#include "Arity.h"
#include "SymbolAttrTable.h"
#include "KeywordTable.h"
#include "KeywordType.h"

#define   fudOf(index)                (keywordTable[index]).fud

extern SymbolParser *symbolParser;

Symbol *statement();
Symbol *cDecl(int rbp);
Symbol *typeFud(Symbol *symbol);
void verifyIsSymbolInTable(SymbolParser *symbolParser, Symbol *symbol);
int verifyIsSymbolKeywordType(Symbol *symbol, int keywordType);

#endif // CDECL_H
