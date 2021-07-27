#include "Symbol.h"

OperatorAttrTable operatorIdTable[256] = {
  ['+'] = {{ADD, INC}           , checkDoubleChar},
  ['-'] = {{MINUS, DEC}         , checkDoubleChar},
  ['*'] = {{MULTIPLY, 0}        ,            NULL},
  ['/'] = {{DIVIDE, 0}          ,            NULL},
  ['%'] = {{MODULUS, 0}         ,            NULL},
  ['~'] = {{TILDE, 0}           ,            NULL},
  ['!'] = {{NOT, 0}             ,            NULL},
  ['&'] = {{BIT_AND, LOGI_AND}  , checkDoubleChar},
  ['|'] = {{BIT_OR, LOGI_OR}    , checkDoubleChar},
  ['='] = {{ASSIGNMENT, EQUAL}  , checkDoubleChar},
  /*
  ['('] = {{OPEN_PARENT, 0}     ,            NULL},
  [')'] = {{CLOSE_PARENT, 0}    ,            NULL},
  */
};

extern Tokenizer *tokenizer;
DoubleLinkedList *symbolStack;

//create data structure for Symbol
Symbol *createSymbol(Symbol *leftChild, Token *token, int arity, int id, Symbol *rightChild) {
  Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
  symbol->token = token;
  symbol->arity = arity;
  symbol->id = id;
  symbol->left = leftChild;
  symbol->right = rightChild;
  return symbol;
}

void freeSymbol(void *symbol) {
  if((Symbol *)symbol == NULL)
    return;
  freeSymbol(((Symbol *)symbol)->left);
  freeSymbol(((Symbol *)symbol)->right);
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

Symbol *checkDoubleChar(Token *symbol) {
  Token *nextSymbol;
  nextSymbol = getToken(tokenizer);
  if(isNULLToken(nextSymbol) || !(isSymbolSameAndAdjacent(symbol, nextSymbol))) {
    pushBackToken(tokenizer, nextSymbol);
    return createSymbol(NULL, symbol, INFIX, operatorIdTable[(symbol->str)[0]].type[0], NULL);
  //if both symbols are same and adjacent to each other
  }else {
    freeToken(nextSymbol);
    (symbol->length)++;
    char newStr[3] = {(symbol->str)[0], (symbol->str)[0], '\0'};
    free(symbol->str);
    symbol->str = createString(newStr);
    return createSymbol(NULL, symbol, INFIX, operatorIdTable[(symbol->str)[0]].type[1], NULL);
  }
}

Symbol *_getSymbol(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  if(isIdentifierToken(token))
    return createSymbol(NULL, token, INFIX, VARIABLE, NULL);
  else if(isIntegerToken(token))
    return createSymbol(NULL, token, INFIX, NUMBER, NULL);
  else if(isNULLToken(token))
    return createSymbol(NULL, token, INFIX, EOL, NULL);
  else if(operatorIdTable[(token->str)[0]].func == NULL)
    return createSymbol(NULL, token, INFIX, operatorIdTable[(token->str)[0]].type[0], NULL);
  else
    return operatorIdTable[(token->str)[0]].func(token);
}

Symbol *peekSymbol(Tokenizer *tokenizer) {
  if(symbolStack->count == 0)
    pushStack(symbolStack, _getSymbol(tokenizer));
  return peekStack(symbolStack);
}

Symbol *getSymbol(Tokenizer *tokenizer) {
  if(symbolStack->count == 0)
    return _getSymbol(tokenizer);
  else
    return popStack(symbolStack);
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