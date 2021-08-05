#include "Symbol.h"

OperatorAttrTable operatorIdTable[] = {
  ['+'] = {{ADD, INC, SH_ADD, 0}           , checkDoubleSameChar},
  ['-'] = {{MINUS, DEC, SH_MINUS, 0}         , checkDoubleSameChar},
  ['*'] = {{MULTIPLY, 0, SH_MUL, 0}        , checkEqualAsLastChar},
  ['/'] = {{DIVIDE, 0, SH_DIV, 0}          , checkEqualAsLastChar},
  ['%'] = {{MODULUS, 0, SH_MOD, 0}         , checkEqualAsLastChar},
  ['~'] = {{BIT_NOT, 0, 0, 0}           , NULL},
  ['!'] = {{NOT, 0, NOT_EQUAL, 0}             , checkEqualAsLastChar},
  ['&'] = {{BIT_AND, LOGI_AND, SH_BIT_AND, 0}  , checkDoubleSameChar},
  ['|'] = {{BIT_OR, LOGI_OR, SH_BIT_OR, 0}    , checkDoubleSameChar},
  ['^'] = {{BIT_XOR, 0, SH_BIT_XOR, 0}, checkEqualAsLastChar},
  ['='] = {{ASSIGNMENT, 0, EQUALITY, 0}  , checkEqualAsLastChar},
  ['<'] = {{LESSER, L_SHIFT, LESS_OR_EQUAL, SH_L_SHIFT}, checkDoubleSameCharWithEqual},
  ['>'] = {{GREATER, R_SHIFT, GREATER_OR_EQUAL, SH_R_SHIFT}, checkDoubleSameCharWithEqual}, 
  ['('] = {{OPEN_PARENT, 0, 0, 0}     , NULL},
  [')'] = {{CLOSE_PARENT, 0, 0, 0}    , NULL},
  [';'] = {{EOL, 0, 0, 0}             , NULL},
};

SymbolCombination SymbolCombiTable[] = {
  [DOUBLE]     = {NULL, 1, DOUBLE_SAME_CHAR},
  [EQUAL]      = {"=",  2, EQUAL_AS_LAST_CHAR},
  [DWITHEQUAL] = {"=",  3, DOUBLE_SAME_CHAR | EQUAL_AS_LAST_CHAR},
};

ArityMemory arityMemoryTable[] = {
  [PREFIX]      =  1,
  [INFIX]       =  2,
  [SUFFIX]      =  1,
  [IDENTITY]    =  0,
};
  
DoubleLinkedList *symbolStack;

//create data structure for Symbol
Symbol *createSymbol(Symbol *symbolInfo) {
  Symbol *symbol = (Symbol *)malloc(sizeof(Symbol) + 
                                    (arityMemoryTable[symbolInfo->arity].extraMemory)*sizeof(Symbol *));
  symbol->token = symbolInfo->token;
  symbol->arity = symbolInfo->arity;
  symbol->id = symbolInfo->id;
  for(int i = 0; i < arityMemoryTable[symbolInfo->arity].extraMemory; i++)
    symbol->child[i] = NULL;
  return symbol;
}

void freeSymbol(void *symbol) {
  if((Symbol *)symbol == NULL)
    return;
  freeSymbol(((Symbol *)symbol)->child[0]);
  freeSymbol(((Symbol *)symbol)->child[1]);
  if(((Symbol *)symbol)->token)
    freeToken(((Symbol *)symbol)->token);
  free((Symbol *)symbol);
}

//return a malloc'ed string
char *createString(char *str) {
  char *newStr;
  int len;
  if(str) {
    len = strlen(str);
    newStr = malloc(len+1);
    strncpy(newStr, str, len);
    newStr[len] = '\0';
    return newStr;
  }else
    return NULL;
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

Symbol *processSymbol(Token *symbol, int *flag, int option, int type) {
  Token *nextSymbol = getToken(tokenizer);
  Symbol symbolInfo = {INFIX, operatorIdTable[(symbol->str)[0]].type[type], symbol};
  if(isNULLToken(nextSymbol) || !(isCorrectSymbolAndAdjacent(symbol, nextSymbol, SymbolCombiTable[option].symbol)))
    pushBackToken(tokenizer, nextSymbol);
  else {
    if(flag)
      *flag = SymbolCombiTable[option].flag;
    freeToken(nextSymbol);
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[SymbolCombiTable[option].type];
    symbolInfo.token = processToken(symbol, option);
  }
  return createSymbol(&symbolInfo);
}

int isCorrectSymbolAndAdjacent(Token *symbol, Token *nextSymbol, char *symToCheck) {
  if(!symToCheck)
    symToCheck = symbol->str;
  return (isToken(symToCheck, nextSymbol) && (nextSymbol->startColumn == (symbol->startColumn + symbol->length)));
}

Symbol *checkEqualAsLastChar(Token *symbol, int *flag) {
  return processSymbol(symbol, flag, EQUAL, 0);
}

Symbol *checkDoubleSameChar(Token *symbol, int *flag) {
  Symbol *newSymbol = checkEqualAsLastChar(symbol, flag);
  if(*flag != EQUAL_AS_LAST_CHAR)
    return processSymbol(symbol, flag, DOUBLE, 0);
  else
    return newSymbol;
}

Symbol *checkDoubleSameCharWithEqual(Token *symbol, int *flag) {
  Symbol *newSymbol = checkDoubleSameChar(symbol, flag);
  if(*flag == DOUBLE_SAME_CHAR)
    return processSymbol(symbol, flag, DWITHEQUAL, 1);
  else
    return newSymbol;
}

Symbol *_getSymbol(Tokenizer *tokenizer) {
  Token *symbol;
  int flagVal = 0;
  int *flag = &flagVal;
  symbol = getToken(tokenizer);
  Symbol symbolInfo = {INFIX, EOL, symbol};
  if(isNULLToken(symbol))
    return createSymbol(&symbolInfo);
  else if(isIdentifierToken(symbol))
    symbolInfo.id = VARIABLE;
  else if(isIntegerToken(symbol))
    symbolInfo.id = NUMBER;
  else if(!(hasSymbolVariations(symbol)))
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[0];
  else
    return operatorIdTable[(symbol->str)[0]].func(symbol, flag);
  return createSymbol(&symbolInfo);
}

Symbol *getSymbol(Tokenizer *tokenizer) {
  if(symbolStack->count == 0)
    return _getSymbol(tokenizer);
  else
    return popStack(symbolStack);
}

Symbol *peekSymbol(Tokenizer *tokenizer) {
  if(symbolStack->count == 0)
    pushStack(symbolStack, _getSymbol(tokenizer));
  return peekStack(symbolStack);
}

Symbol *popStack(DoubleLinkedList *stack) {
  return (linkedListRemoveItemFromHead(stack))->data;
}

void pushStack(DoubleLinkedList *stack, Symbol *symbol) {
  ListItem *symbolPtr = linkedListCreateListItem(symbol);
  linkedListAddItemToHead(symbolPtr, stack);
}

Symbol *peekStack(DoubleLinkedList *stack) {
  return stack->head->data;
}

void verifyIsSymbolThenConsume(char *symToCheck, Symbol *symbol) {
  if(isNULLToken(symbol->token) || !(isToken(symToCheck, symbol->token))) {
    throwException(ERR_WRONG_SYMBOL, symbol->token, 0,
    "Expecting a %s here, but received %s instead", symToCheck, symbol->token->str);
  }
  else
    freeSymbol(getSymbol(tokenizer));
}