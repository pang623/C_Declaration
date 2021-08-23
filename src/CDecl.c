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

int isSymbolKeywordType(Symbol *symbol, int keywordType, int *index) {
  if(!isIdentifierToken(symbol->token))
    return 0;
  int i = 0;
  while(strcmp(symbol->token->str, keywordTable[i++].keyword))
    if(keywordTable[i].keyword == NULL) {
      *index = i;
      return 1;
    }
  if(keywordTable[i].type != keywordType)
    return 0;
  else {
    *index = i;
    return 1;
  }
}

int verifyIsSymbolKeywordType(Symbol *symbol, int keywordType) {
  int *index;
  if(isSymbolKeywordType(symbol, keywordType, &index))
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
  while(rbp < getInfixLBP(peekSymbol(tokenizer))) {
    symbol = getSymbol(tokenizer);
    verifyIsSymbolInTable(symbolParser, symbol);
    left = ledOf(symbol)(symbol, left);
  }
  return left;
}

Symbol *typeFud(Symbol *symbol) {
  Symbol *left = symbol;
  left->id = TYPE;
  left->child[0] = cDecl(0);
  return left;
}

Symbol *sqrBracketLed(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  symbol->child[1] = expression(0);
  //verifyHasNoIdentifier(symbol->child[1]);
  verifyIsNextSymbolThenConsume(symbolParser->tokenizer, CLOSE_SQR, "]");
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

/*
void verifyHasNoIdentifier(Symbol *symbol) {
  if(symbol == NULL)
    return;
  if(symbol->id == IDENTIFIER)
    throwException(ERR_ARRAY_SUBSCRIPT, symbol->token, 0,
    "Variable cannot be used in array subscripting", symbol->token->str);
  verifyHasNoIdentifier(symbol->child[0]);
  verifyHasNoIdentifier(symbol->child[1]);
}
*/