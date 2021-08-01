#include "Symbol.h"

OperatorAttrTable operatorIdTable[] = {
  ['+'] = {{ADD, INC}           , checkDoubleSameChar},
  ['-'] = {{MINUS, DEC}         , checkDoubleSameChar},
  ['*'] = {{MULTIPLY, 0}        , NULL},
  ['/'] = {{DIVIDE, 0}          , NULL},
  ['%'] = {{MODULUS, 0}         , NULL},
  ['~'] = {{TILDE, 0}           , NULL},
  ['!'] = {{NOT, 0}             , NULL},
  ['&'] = {{BIT_AND, LOGI_AND}  , checkDoubleSameChar},
  ['|'] = {{BIT_OR, LOGI_OR}    , checkDoubleSameChar},
  ['='] = {{ASSIGNMENT, EQUAL}  , checkDoubleSameChar},
  ['('] = {{OPEN_PARENT, 0}     , NULL},
  [')'] = {{CLOSE_PARENT, 0}    , NULL},
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

Symbol *checkDoubleSameChar(Token *symbol) {
  Token *nextSymbol;
  Symbol symbolInfo = {INFIX, operatorIdTable[(symbol->str)[0]].type[0], symbol};
  nextSymbol = getToken(tokenizer);
  if(isNULLToken(nextSymbol) || !(isSymbolSameAndAdjacent(symbol, nextSymbol)))
    pushBackToken(tokenizer, nextSymbol);
  //if both symbols are same and adjacent to each other
  else {
    freeToken(nextSymbol);
    (symbol->length)++;
    char newStr[3] = {(symbol->str)[0], (symbol->str)[0], '\0'};
    free(symbol->str);
    symbol->str = createString(newStr);
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[1];
  }
  return createSymbol(&symbolInfo);
}

Symbol *_getSymbol(Tokenizer *tokenizer) {
  Token *symbol;
  symbol = getToken(tokenizer);
  Symbol symbolInfo = {INFIX, EOL, symbol};
  if(isNULLToken(symbol) || isToken(";", symbol))
    return createSymbol(&symbolInfo);
  else if(isIdentifierToken(symbol))
    symbolInfo.id = VARIABLE;
  else if(isIntegerToken(symbol))
    symbolInfo.id = NUMBER;
  else if(!(hasSymbolVariations(symbol)))
    symbolInfo.id = operatorIdTable[(symbol->str)[0]].type[0];
  else
    return operatorIdTable[(symbol->str)[0]].func(symbol);
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
  if(isNULLToken(symbol->token) || !isSymbol(symToCheck, symbol)) {
    throwException(ERR_WRONG_SYMBOL, symbol->token, 0,
    "Expecting a %s here, but received %s instead", symToCheck, symbol->token->str);
  }
  else
    freeSymbol(getSymbol(tokenizer));
}