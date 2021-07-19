#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable symbolTable[256] = {
  [NUMBER]    =   { 1,  1,  1,   identityNud,  identityLed},
  [VARIABLE]  =   { 1,  1,  1,   identityNud,  identityLed},
  [ADD]       =   {50, 30, 30,     prefixNud,    infixLedL},
  [MINUS]     =   {50, 30, 30,     prefixNud,    infixLedL},
  [MULTIPLY]  =   {50, 40, 40,     prefixNud,    infixLedL},
  [DIVIDE]    =   { 0, 40, 40,      errorNud,    infixLedL},
  [MODULUS]   =   { 0, 40, 40,      errorNud,    infixLedL},
  [TILDE]     =   {50,  0,  0,     prefixNud,     errorLed},
  [NOT]       =   {50,  0,  0,     prefixNud,     errorLed},
  [INC]       =   {50,  0, 60,  prefixNudTwo,    suffixLed},
  [DEC]       =   {50,  0, 60,  prefixNudTwo,    suffixLed},
};

int operatorIdTable[256] = {
  ['+'] = ADD,
  ['-'] = MINUS,
  ['*'] = MULTIPLY,
  ['/'] = DIVIDE,
  ['%'] = MODULUS,
  ['~'] = TILDE,
  ['!'] = NOT,
};

Tokenizer *tokenizer;

//create data structure for Symbol
Symbol *createSymbol(Symbol *leftChild, Token *token, int arity, Symbol *rightChild) {
  Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
  symbol->token = token;
  symbol->arity = arity;
  symbol->left = leftChild;
  symbol->right = rightChild;
  return symbol;
}

void freeSymbol(Symbol *symbol) {
  if(symbol == NULL)
    return;
  freeSymbol(symbol->left);
  freeSymbol(symbol->right);
  freeToken(symbol->token);
  free(symbol);
}

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

//only peeks current token, does not consume it
Token *peek(Tokenizer *tokenizer) {
  Token *token = NULL;
  Token *token1, *token2;
  token1 = getToken(tokenizer);
  if(isNULLToken(token1)) {
    token = token1;
    pushBackToken(tokenizer, token1);
    return token;
  }
  if((token1->str)[0] == '+') {
    token2 = getToken(tokenizer);
    if((token2->str)[0] == '+')
      token = (Token *)createOperatorToken(createString("++"), tokenizer->index, tokenizer->str, TOKEN_OPERATOR_TYPE);
    else
      token = token1;
    pushBackToken(tokenizer, token2);
  }else if((token1->str)[0] == '-') {
    token2 = getToken(tokenizer);
    if((token2->str)[0] == '-')
      token = (Token *)createOperatorToken(createString("--"), tokenizer->index, tokenizer->str, TOKEN_OPERATOR_TYPE);
    else
      token = token1;
    pushBackToken(tokenizer, token2);
  }else
    token = token1;
  pushBackToken(tokenizer, token1);
  return token;
}

//handles prefix
//unary etc
Symbol *prefixNud() {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(NULL, token, PREFIX, expression(getPrefixRBP(token)));
}

//for prefix operators with two symbols, eg: "++", "--"
Symbol *prefixNudTwo() {
  Token *token = NULL;
  Token *spare;
  token = peek(tokenizer);
  spare = getToken(tokenizer);
  spare = getToken(tokenizer);
  free(spare);
  return createSymbol(NULL, token, PREFIX, expression(getPrefixRBP(token)));
}

//handles infix and postfix
//called when operator binds to the left
//for left associativity
Symbol *infixLedL(Symbol *left) {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(left, token, INFIX, expression(getInfixRBP(token)));
}

//for right associativity
Symbol *infixLedR(Symbol *left) {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(left, token, INFIX, expression(getInfixRBP(token) - 1));
}

//postfix, eg: "++", "--"
Symbol *suffixLed(Symbol *left) {
  Token *token = NULL;
  Token *spare;
  token = peek(tokenizer);
  if(isIncToken(left->token) || isDecToken(left->token))
    throwException(ERR_SYNTAX, token, 0,
    "Does not expect suffix %s here", token->str);
  spare = getToken(tokenizer);
  spare = getToken(tokenizer);
  free(spare);
  return createSymbol(left, token, SUFFIX, NULL);
}

//just returns the symbol (numbers, var)
Symbol *identityNud() {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(NULL, token, IDENTITY, NULL);
}

//error handling for illegal prefix
Symbol *errorNud() {
  Token *token = NULL;
  token = getToken(tokenizer);
  throwException(ERR_SYNTAX, token, 0,
  "Operator %s is not a unary operator", token->str);
}

//error handling for illegal infix
Symbol *errorLed(Symbol *left) {
  Token *token = NULL;
  token = getToken(tokenizer);
  throwException(ERR_SYNTAX, token, 0,
  "Operator %s is not a binary operator", token->str);
}

//error handling for numbers and variables 
//they cannot be infix, thus if led is called an error is thrown
Symbol *identityLed(Symbol *left) {
  Token *token = NULL;
  token = getToken(tokenizer);
  throwException(ERR_EXPECTED_OPERATOR, token, 0,
  "Expected an operator here, but received %s instead", token->str);
}

int getSymbolId(Token *token) {
  if(isIdentifierToken(token))
    return VARIABLE;
  else if(isIntegerToken(token))
    return NUMBER;
  else if(isNULLToken(token))
    return -1;
  else if(isIncToken(token))
    return INC;
  else if(isDecToken(token))
    return DEC;
  else
    return operatorIdTable[(token->str)[0]];
}

//main parser
Symbol *expression(int rbp) {
  Symbol *left;
  //maybe put nud and led to a macro
  left = symbolTable[getSymbolId(peek(tokenizer))].nud();
  while(rbp < getInfixLBP(peek(tokenizer)))
    left = symbolTable[getSymbolId(peek(tokenizer))].led(left);
  return left;
}