#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Exception.h"
#include "CDecl_Errors.h"
#include "Arity.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "KeywordTable.h"
#include "KeywordType.h"

#define   NIL   1000

//Macros
#define   getInfixLBP(symbol)         ((symbolParser->symbolTable)[symbol->id].infixLBP)
#define   getInfixRBP(symbol)         ((symbolParser->symbolTable)[symbol->id].infixRBP)
#define   getPrefixRBP(symbol)        ((symbolParser->symbolTable)[symbol->id].prefixRBP)

#define   nudOf(symbol)               ((symbolParser->symbolTable)[(symbol)->id]).nud
#define   ledOf(symbol)               ((symbolParser->symbolTable)[(symbol)->id]).led

extern SymbolParser *symbolParser;

Symbol *expression(int rbp);
Symbol *prefixNud(Symbol *symbol);
Symbol *infixLedL(Symbol *symbol, Symbol *left);
Symbol *infixLedR(Symbol *symbol, Symbol *left);
Symbol *suffixLed(Symbol *symbol, Symbol *left);
Symbol *identityNud(Symbol *symbol);
Symbol *errorNud(Symbol *symbol);
Symbol *missingOperandNud(Symbol *symbol);
Symbol *errorLed(Symbol *symbol, Symbol *left);
Symbol *identityLed(Symbol *symbol, Symbol *left);
Symbol *parentNud(Symbol *symbol);
Symbol *sqrBracketLed(Symbol *symbol, Symbol *left);
Symbol *parse(int rbp);
int isSymbolKeywordType(Symbol *symbol, int keywordType, int *index);

#endif // EXPRESSIONPARSER_H
