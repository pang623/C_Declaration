#include "Symbol.h"
#include <string.h>

OperatorAttrTable operatorIdTable[256] = {
  ['+'] = {{ADD, INC_BEFORE, ADD_ASSIGN, 0}                        ,     handleRepeatedAndEqualSymbol},
  ['-'] = {{SUBTRACT, DEC_BEFORE, SUBT_ASSIGN, 0}                  ,     handleRepeatedAndEqualSymbol},
  ['*'] = {{MULTIPLY, 0, MUL_ASSIGN, 0}                            ,                handleEqualSymbol},
  ['/'] = {{DIVIDE, 0, DIV_ASSIGN, 0}                              ,                handleEqualSymbol},
  ['%'] = {{MODULUS, 0, MOD_ASSIGN, 0}                             ,                handleEqualSymbol},
  ['!'] = {{LOGI_NOT, 0, NOT_EQUAL, 0}                             ,                handleEqualSymbol},
  ['&'] = {{BIT_AND, LOGI_AND, AND_ASSIGN, 0}                      ,     handleRepeatedAndEqualSymbol},
  ['|'] = {{BIT_OR, LOGI_OR, OR_ASSIGN, 0}                         ,     handleRepeatedAndEqualSymbol},
  ['^'] = {{BIT_XOR, 0, XOR_ASSIGN, 0}                             ,                handleEqualSymbol},
  ['='] = {{ASSIGNMENT, 0, EQUALITY, 0}                            ,                handleEqualSymbol},
  ['<'] = {{LESSER, L_SHIFT, LESS_OR_EQUAL, L_SHIFT_ASSIGN}        , handleEqualRepeatedAndBothSymbol},
  ['>'] = {{GREATER, R_SHIFT, GREATER_OR_EQUAL, R_SHIFT_ASSIGN}    , handleEqualRepeatedAndBothSymbol}, 
  ['~'] = {{BIT_NOT, 0, 0, 0}                                      ,                             NULL},
  ['('] = {{OPEN_PARENT, 0, 0, 0}                                  ,                             NULL},
  [')'] = {{CLOSE_PARENT, 0, 0, 0}                                 ,                             NULL},
  ['['] = {{OPEN_SQR, 0, 0, 0}                                     ,                             NULL},
  [']'] = {{CLOSE_SQR, 0, 0, 0}                                    ,                             NULL},
  [','] = {{COMMA, 0, 0, 0}                                        ,                             NULL},
  [';'] = {{EOL, 0, 0, 0}                                          ,                             NULL},
};

KeywordAttrTable keywordIdTable[] = {
  {"int"        , TYPE},
  {"char"       , TYPE},
  {"float"      , TYPE},
  {"double"     , TYPE},
  {"void"       , TYPE},
  {"if"         , IF},
  {"else"       , IF},
  {"while"      , WHILE},
  {"for"        , FOR},
  {"do"         , WHILE},
  {"switch"     , CASE},
  {"case"       , CASE},
  {"continue"   , CONTINUE},
  {"break"      , BREAK},
  {NULL         , TYPE},      //non-keyword identifiers are assumed as user-defined data type
};

SymbolCombination SymbolCombiTable[] = {
  [DOUBLE]     = {NULL, 1, DOUBLE_SAME_CHAR},
  [EQUAL]      = {"=",  2, EQUAL_AS_LAST_CHAR},
  [DWITHEQUAL] = {"=",  3, DOUBLE_SAME_CHAR | EQUAL_AS_LAST_CHAR},
};

int isSymbolKeyword(Symbol *symbol, int identifierIsSeenAsKeyword) {
  int i = 0;
  int *type = &i;
  return isSymbolKeywordThenGetType(symbol, type, identifierIsSeenAsKeyword);
}

int isSymbolKeywordThenGetType(Symbol *symbol, int *type, int identifierIsSeenAsKeyword) {
  if(!isIdentifierToken(symbol->token))
    return 0;
  int i = 0;
  while(strcmp(symbol->token->str, keywordIdTable[i].keyword)) {
    i++;
    if(keywordIdTable[i].keyword == NULL) {
      if(!identifierIsSeenAsKeyword)
        return 0;
      break;
    }
  }
  *type = keywordIdTable[i].type;
  return 1;
}

SymbolParser *createSymbolParser(Tokenizer *tokenizer) {
  SymbolParser *parser = (SymbolParser *)malloc(sizeof(SymbolParser));
  while(parser == NULL)
    parser = (SymbolParser *)malloc(sizeof(SymbolParser));
  parser->tokenizer = tokenizer;
  parser->symbolStack = linkedListCreateList();
  parser->symbolTable = NULL;
  return parser;
}

void freeSymbolParser(SymbolParser *symbolParser) {
  if(symbolParser == NULL)
    return;
  if(symbolParser->tokenizer)
    freeTokenizer(symbolParser->tokenizer);
  if(symbolParser->symbolStack)
    linkedListFreeList(symbolParser->symbolStack, freeSymbol);
  free(symbolParser);
}

void setSymbolTable(SymbolParser *symbolParser, SymbolAttrTable *table) {
  symbolParser->symbolTable = table;
}

//create data structure for Symbol
Symbol *createSymbol(Symbol *symbolInfo) {
  Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
  while(symbol == NULL)
    symbol = (Symbol *)malloc(sizeof(Symbol));
  symbol->token = symbolInfo->token;
  symbol->arity = symbolInfo->arity;
  symbol->id = symbolInfo->id;
  symbol->child[0] = NULL;
  symbol->child[1] = NULL;
  return symbol;
}

void freeSymbol(void *symbol) {
  if((Symbol *)symbol == NULL)
    return;
  freeSymbol(((Symbol *)symbol)->child[0]);
  freeSymbol(((Symbol *)symbol)->child[1]);
  if(((Symbol *)symbol)->token) {
    freeToken(((Symbol *)symbol)->token);
    ((Symbol *)symbol)->token = NULL;
  }
  free((Symbol *)symbol);
}

Token *processToken(Token *symbol, int option) {
  (symbol->length)++;
  char newStr[symbol->length];
  for(int i = 0; i < symbol->length; i++)
    newStr[i] = (symbol->str)[0];
  newStr[symbol->length] = '\0';
  if(SymbolCombiTable[option].symbol)
    newStr[symbol->length - 1] = (SymbolCombiTable[option].symbol)[0];
  free(symbol->str);
  symbol->str = createString(newStr);
  return symbol;
}

Symbol processSymbol(Token *symbol, int *flag, int option, int type) {
  Token *nextSymbol = getToken(symbolParser->tokenizer);
  Symbol symbolInfo = {INFIX, operatorIdTable[(symbol->str)[0]].type[type], symbol};
  if(isNULLToken(nextSymbol) || !(isCorrectSymbolAndAdjacent(symbol, nextSymbol, SymbolCombiTable[option].symbol)))
    pushBackToken(symbolParser->tokenizer, nextSymbol);
  else {
    if(flag)
      *flag = SymbolCombiTable[option].flag;
    freeToken(nextSymbol);
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[SymbolCombiTable[option].type];
    symbolInfo.token = processToken(symbol, option);
  }
  return symbolInfo;
}

int isCorrectSymbolAndAdjacent(Token *symbol, Token *nextSymbol, char *symToCheck) {
  if(!symToCheck)
    symToCheck = symbol->str;
  return (isToken(symToCheck, nextSymbol) && (nextSymbol->startColumn == (symbol->startColumn + symbol->length)));
}

Symbol handleEqualSymbol(Token *symbol, int *flag) {
  return processSymbol(symbol, flag, EQUAL, 0);
}

Symbol handleRepeatedAndEqualSymbol(Token *symbol, int *flag) {
  Symbol newSymbol = handleEqualSymbol(symbol, flag);
  if(*flag != EQUAL_AS_LAST_CHAR)
    return processSymbol(symbol, flag, DOUBLE, 0);
  else
    return newSymbol;
}

Symbol handleEqualRepeatedAndBothSymbol(Token *symbol, int *flag) {
  Symbol newSymbol = handleRepeatedAndEqualSymbol(symbol, flag);
  if(*flag == DOUBLE_SAME_CHAR)
    return processSymbol(symbol, flag, DWITHEQUAL, 1);
  else
    return newSymbol;
}

Symbol *_getSymbol(Tokenizer *tokenizer) {
  int flagVal = 0;
  int *flag = &flagVal;
  Token *symbol = getToken(tokenizer);
  Symbol symbolInfo = {INFIX, EOL, symbol};
  if(isNULLToken(symbol))
    return createSymbol(&symbolInfo);
  else if(isIdentifierToken(symbol))
    symbolInfo.id = IDENTIFIER;
  else if(isIntegerToken(symbol) || isFloatToken(symbol))
    symbolInfo.id = NUMBER;
  else if(!(hasSymbolVariations(symbol)))
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[0];
  else
    symbolInfo = operatorIdTable[(symbol->str)[0]].func(symbol, flag);
  
  //Symbols that are not defined in the table
  if(symbolInfo.id == UNKNOWN)
    throwException(ERR_INVALID_SYMBOL, symbolInfo.token, 1,
    "Symbol '%s' is not supported in C Language", (symbolInfo.token)->str);
  return createSymbol(&symbolInfo);
}

Symbol *getSymbol(SymbolParser *symbolParser) {
  if(symbolParser->symbolStack->count == 0)
    return _getSymbol(symbolParser->tokenizer);
  else
    return popStack(symbolParser->symbolStack);
}

Symbol *peekSymbol(SymbolParser *symbolParser) {
  if(symbolParser->symbolStack->count == 0)
    pushStack(symbolParser->symbolStack, linkedListCreateListItem((Symbol *)_getSymbol(symbolParser->tokenizer)));
  return peekStack(symbolParser->symbolStack);
}

int isNextSymbolThenConsume(SymbolID symbolId) {
  Symbol *symbol = getSymbol(symbolParser);
  if(symbol->id == symbolId) {
    freeSymbol(symbol);
    return 1;
  }else {
    pushStack(symbolParser->symbolStack, linkedListCreateListItem((Symbol *)symbol));
    return 0;
  }
}

void verifyIsNextSymbolThenConsume(SymbolID symbolId, char *expectedSym) {
  if(!(isNextSymbolThenConsume(symbolId))) {
    Symbol *symbol = getSymbol(symbolParser);
    throwException(ERR_WRONG_SYMBOL, symbol->token, 0,
    "Expecting a '%s' here, but received '%s' instead", expectedSym, symbol->token->str);
  }
}