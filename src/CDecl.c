#include "CDecl.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable CDeclSymbolTable[256] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,     nud,     led}
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,           identityNud,  identityLed},
  //[MULTIPLY]           =   { 140,  NIL,  NIL,             pointerNud,    pointerLed},
  //[OPEN_PARENT]        =   {  0,   NIL,  NIL,             parentNud,     errorLed},
  //[CLOSE_PARENT]       =   {  0,     0,    0,              errorNud,         NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,              errorNud,  sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,  NULL},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,         NULL},
};

StatementKeywordTable keywordTable[] = {
  //{"KEYWORD"  , KEYWORD_TYPE, fud}
  {"int"        , TYPE, typeFud},
  {"char"       , TYPE, typeFud},
  {"float"      , TYPE, typeFud},
  {"double"     , TYPE, typeFud},
  {"if"         , FLOW,    NULL},
  {"else"       , FLOW,    NULL},
  {"while"      , FLOW,    NULL},
  {"for"        , FLOW,    NULL},
  {"do"         , FLOW,    NULL},
  {"switch"     , FLOW,    NULL},
  {"case"       , FLOW,    NULL},
  {"continue"   , FLOW,    NULL},
  {"break"      , FLOW,    NULL},
  {NULL         , TYPE, typeFud},
};

char *errorStrings[] = {
  [TYPE] = "data",
  [FLOW] = "flow control",
};

int verifyIsSymbolKeywordType(Symbol *symbol, int keywordType) {
  int num = 0;
  int *index = &num;
  if(isSymbolKeywordType(symbol, keywordType, index))
    return *index;
  else
    throwException(ERR_KEYWORD, symbol->token, 0,
    "Expecting a keyword of %s type here", errorStrings[keywordType]);
}

void verifyIsSymbolInTable(SymbolParser *symbolParser, Symbol *symbol) {
  if(nudOf(symbol) == NULL && ledOf(symbol) == NULL)
    throwException(ERR_INVALID_SYMBOL, symbol->token, 0,
    "Symbol %s is not supported here", symbol->token->str);
}

Symbol *cDecl(int rbp) {
  Symbol *left, *symbol;
  setSymbolTable(symbolParser, CDeclSymbolTable);
  symbol = getSymbol(symbolParser);
  verifyIsSymbolInTable(symbolParser, symbol);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP(peekSymbol(symbolParser->tokenizer))) {
    symbol = getSymbol(symbolParser);
    verifyIsSymbolInTable(symbolParser, symbol);
    left = ledOf(symbol)(symbol, left);
  }
  return left;
}

Symbol *typeFud(Symbol *symbol) {
  symbol->id = TYPE;
  symbol->arity = IDENTITY;
  symbol->child[0] = cDecl(0);
  return symbol;
}

Symbol *statement() {
  Symbol *left;
  Symbol *symbol = getSymbol(symbolParser);
  int index = verifyIsSymbolKeywordType(symbol, TYPE);
  left = fudOf(index)(symbol);
  verifyIsNextSymbolThenConsume(symbolParser->tokenizer, EOL, ";");
  return left;
}