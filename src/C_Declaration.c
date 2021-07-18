#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

//left associativity, rbp > lbp
//right associativity, rbp < lbp

SymbolAttrTable symbolTable[256] = {
  [ADD]       =   {50, 10, 10,   prefixNud,    infixLed},
  [MINUS]     =   {50, 10, 10,   prefixNud,    infixLed},
  [MULTIPLY]  =   { 0, 30, 30,    errorNud,    infixLed},
  [DIVIDE]    =   { 0, 20, 20,    errorNud,    infixLed},
  [TILDE]     =   {50,  0,  0,   prefixNud,    errorLed},
  [NUMBER]    =   { 0,  0,  0, identityNud, identityLed},
  [VARIABLE]  =   { 0,  0,  0, identityNud, identityLed},
};

int operatorIdTable[256] = {
  ['+'] = ADD,
  ['-'] = MINUS,
  ['*'] = MULTIPLY,
  ['/'] = DIVIDE,
  ['~'] = TILDE,
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

//only peeks current token, does not consume it
Token *peek(Tokenizer *tokenizer) {
  Token *token = NULL;
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  return token;
}

//handles prefix
Symbol *prefixNud() {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(NULL, token, PREFIX, expression(getPrefixRBP(token)));
}

//handles infix and postfix
//called when operator binds to the left
Symbol *infixLed(Symbol *left) {
  Token *token = NULL;
  token = getToken(tokenizer);
  return createSymbol(left, token, INFIX, expression(getInfixRBP(token)));
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
  else if(isIntegerToken(token) || isFloatToken(token))
    return NUMBER;
  else if(isNULLToken(token))
    return -1;
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