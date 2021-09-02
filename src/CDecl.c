#include "CDecl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolAttrTable CDeclSymbolTable[256] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,      nud,           led}
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,           identityNud,   identityLed},
  [MULTIPLY]           =   { 140,  NIL,  NIL,            pointerNud,    pointerLed},
  [OPEN_PARENT]        =   {   0,    0,  150,           groupingNud,       funcLed},
  [CLOSE_PARENT]       =   {   0,    0,    0,              errorNud,          NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,              errorNud, sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,          NULL},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,          NULL},
};

SymbolAttrTable MultiCDeclSymbolTable[256] = {
  //COMMA indicates there's still C declaration to be read
  [COMMA]              =   {  0,     0,    0,     missingOperandNud,      commaLed},
  //CLOSE_PARENT acts as terminator, no C declaration after this
  [CLOSE_PARENT]       =   {  0,     0,    0,              errorNud,          NULL},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,          NULL},
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
  [IDENTIFIER]      = " is ",
  [NUMBER]          = " of ",
  [OPEN_SQR]        = "array of ",
  //[FUNCTION]        = "function taking in",
  [POINTER]         = "pointer to ",
  [COMMA]           = "",
  [TYPE]            = "",
  [OPEN_PARENT]     = "",
};

int multipleDecl = 0;
/*
char *functionRead(Symbol *symbol) {
  char *str;
  //if no function parameters
  if(symbol->child[1] == NULL)
    str = createString("function ");
  else
    str = createString("function taking in ");
  concat(str, createString(symbol->token->str));
  str = readAST(symbol->child[1], str);
  return concat(str, createString(") returning "));
}

char *ptrRead(Symbol *symbol) {
  char *str = createString("pointer to ");
  return str;
}

char *identifierRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  concat(str, createString(" is ");
  return str;
}

char *generalRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  return str;
}

char *ignoreRead(Symbol *symbol) {
  return createString("");
}

char *arrayRead(Symbol *symbol) {
  char *str = createString("array of ");
  str = readAST(symbol->child[1], str);
  concat(str, createString(" of "));
  return str;
}
*/

Symbol *groupingNud(Symbol *symbol) {
  symbol->child[0] = cDecl(0);
  verifyIsNextSymbolThenConsume(CLOSE_PARENT, ")");
  freeSymbol(symbol->child[1]);
  return symbol;
}

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
  SymbolParser *parser = symbolParser;
  Symbol *left, *symbol, *symbolCheck;
  setSymbolTable(parser, CDeclSymbolTable);
  symbol = getSymbol(parser);
  verifyIsSymbolInTable(parser, symbol);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP((symbolCheck = peekSymbol(parser->tokenizer)))) {
    setSymbolTable(parser, CDeclSymbolTable);
    symbol = getSymbol(parser);
    verifyIsSymbolInTable(parser, symbol);
    left = ledOf(symbol)(symbol, left);
  }
  if(multipleDecl)
    setSymbolTable(parser, MultiCDeclSymbolTable);
  else
    setSymbolTable(parser, CDeclSymbolTable);
  verifyIsSymbolInTable(parser, symbolCheck);
  return left;
}

Symbol *typeFud(Symbol *symbol) {
  symbol->id = TYPE;
  symbol->arity = IDENTITY;
  symbol->child[0] = cDecl(0);
  return symbol;
}

Symbol *statements() {
  Symbol *symbol;
  multipleDecl = 1;
  setSymbolTable(symbolParser, MultiCDeclSymbolTable);
  if((peekSymbol(symbolParser->tokenizer))->id == CLOSE_PARENT) {
    multipleDecl = 0;
    return NULL;
  }
  Symbol *left = statement();
  Symbol *next = peekSymbol(symbolParser->tokenizer);
  verifyIsSymbolInTable(symbolParser, next);
  while(next->id == COMMA) {
    symbol = getSymbol(symbolParser);
    left = ledOf(symbol)(symbol, left);
    verifyIsSymbolInTable(symbolParser, next = peekSymbol(symbolParser->tokenizer));
  }
  multipleDecl = 0;
  return left;
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
  if(symbol->id == IDENTIFIER || symbol->id == NUMBER || symbol->id == TYPE)
    return concat(createString(symbol->token->str), createString(ASTtable[symbol->id]));
  else
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