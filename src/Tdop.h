#ifndef TDOP_H
#define TDOP_H

#include "SymbolAttrTable.h"
#include "Symbol.h"
#include "CDecl_Errors.h"

extern SymbolParser *symbolParser;

//Macros
#define   getInfixLBP(symbol)         ((symbolParser->symbolTable)[symbol->id].infixLBP)
#define   getInfixRBP(symbol)         ((symbolParser->symbolTable)[symbol->id].infixRBP)
#define   getPrefixRBP(symbol)        ((symbolParser->symbolTable)[symbol->id].prefixRBP)

#define   nudOf(symbol)               ((symbolParser->symbolTable)[(symbol)->id]).nud
#define   ledOf(symbol)               ((symbolParser->symbolTable)[(symbol)->id]).led

void verifyIsSymbolInTable(SymbolParser *symbolParser, Symbol *symbol);
Symbol *tdop(int rbp, SymbolAttrTable *table);

#endif // TDOP_H
