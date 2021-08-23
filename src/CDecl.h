#ifndef CDECL_H
#define CDECL_H

#include "Symbol.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "ExpressionParser.h"
#include "Arity.h"
#include "SymbolTable.h"

#define   fudOf(index)                (keywordTable[index]).fud

extern SymbolParser *symbolParser;

typedef struct StatementKeywordTable StatementKeywordTable;
typedef Symbol *(*NudFuncPtr)(Symbol *symbol);
typedef Symbol *(*LedFuncPtr)(Symbol *symbol, Symbol *left);
typedef Symbol *(*FudFuncPtr)(Symbol *symbol);

struct StatementKeywordTable {
  char *keyword;
  int type;
  FudFuncPtr fud;
};

Symbol *statement();
Symbol *cDecl(int rbp);
Symbol *typeFud(Symbol *symbol);
Symbol *sqrBracketLed(Symbol *symbol, Symbol *left);
void verifyIsSymbolInTable(SymbolParser *symbolParser, Symbol *symbol);
int isSymbolKeywordType(Symbol *symbol, int keywordType, int *index);
int verifyIsSymbolKeywordType(Symbol *symbol, int keywordType);


#endif // CDECL_H
