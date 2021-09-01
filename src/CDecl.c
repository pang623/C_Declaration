#include "CDecl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolAttrTable CDeclSymbolTable[256] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,      nud,           led}
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,           identityNud,   identityLed},
  [MULTIPLY]           =   { 140,  NIL,  NIL,            pointerNud,    pointerLed},
  [OPEN_PARENT]        =   {   0,    0,  150,             parentNud,       funcLed},
  [CLOSE_PARENT]       =   {   0,    0,    0,              errorNud,          NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,              errorNud, sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,          NULL},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,          NULL},
};

SymbolAttrTable MultiCDeclSymbolTable[256] = {
  //COMMA indicates there's still C declaration to be read
  [COMMA]              =   {  0,    0,    0,      missingOperandNud,      commaLed},
  //CLOSE_PARENT acts as terminator, no C declaration after this
  [CLOSE_PARENT]       =   {  0,     0,    0,                  NULL,          NULL},
  [EOL]                =   {  0,     0,    0,                  NULL,          NULL},
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

char *ASTtable[] = {
  [IDENTIFIER] = " is ",
  [NUMBER] = " of ",
  [OPEN_SQR] = "array of ",
  [POINTER] = "pointer to ",
  [TYPE] = "",
};

Symbol *funcLed(Symbol *symbol, Symbol *left) {
  symbol->id = FUNCTION;
  symbol->child[0] = left;
  symbol->child[1] = statements();
  verifyIsNextSymbolThenConsume(CLOSE_PARENT, ")");
  return symbol;
}

Symbol *commaLed(Symbol *symbol, Symbol *left) {
  symbol->child[0] = left;
  symbol->child[1] = statement();
  return symbol;
}

Symbol *statements() {
  Symbol *symbol;
  setSymbolTable(symbolParser, MultiCDeclSymbolTable);
  Symbol *left = statement();
  Symbol *next = peekSymbol(symbolParser->tokenizer);
  verifyIsSymbolInTable(symbolParser, next);
  while(next->id == COMMA) {
    symbol = getSymbol(symbolParser);
    left = ledOf(symbol)(symbol, left);
    verifyIsSymbolInTable(symbolParser, next = peekSymbol(symbolParser->tokenizer));
  }
  return left;
}

Symbol *pointerLed(Symbol *symbol, Symbol *left) {
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Pointer cannot be used here");
}

Symbol *pointerNud(Symbol *symbol) {
  symbol->arity = PREFIX;
  symbol->id = POINTER;
  symbol->child[0] = cDecl(getPrefixRBP(symbol));
  freeSymbol(symbol->child[1]);
  return symbol;
}

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
  Symbol *left, *symbol, *symbolCheck;
  setSymbolTable(symbolParser, CDeclSymbolTable);
  symbol = getSymbol(symbolParser);
  verifyIsSymbolInTable(symbolParser, symbol);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP((symbolCheck = peekSymbol(symbolParser->tokenizer)))) {
    symbol = getSymbol(symbolParser);
    verifyIsSymbolInTable(symbolParser, symbol);
    left = ledOf(symbol)(symbol, left);
  }
  verifyIsSymbolInTable(symbolParser, symbolCheck);
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
  isNextSymbolThenConsume(EOL);
  return left;
}

char *concat(char *s1, char *s2) {
  char *result = realloc(s1, strlen(s1) + strlen(s2) + 1);
  strcat(result, s2);
  return result;
}

char *readSymbol(Symbol *symbol) {
  //char *src;
  if(symbol->id == IDENTIFIER || symbol->id == NUMBER || symbol->id == TYPE) {
    //src = concat(createString(symbol->token->str), createString(" "));
    return concat(createString(symbol->token->str), createString(ASTtable[symbol->id]));
  }else
    return createString(ASTtable[symbol->id]);
}

char *readAST(Symbol *AST, char *str) {
  if(AST == NULL)
    return str;
  str = readAST(AST->child[0], str);
  str = concat(str, readSymbol(AST));
  str = readAST(AST->child[1], str);
  return str;
}

char *translate(char *cDecl) {
  Tokenizer *tokenizer = createTokenizer(cDecl);
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  char *newStr = readAST(AST, createString(""));
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
  return newStr;
}