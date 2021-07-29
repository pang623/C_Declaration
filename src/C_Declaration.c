#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable symbolTable[256] = {
  [NUMBER]      =   { 1,  1,  1,        identityNud,  identityLed},
  [VARIABLE]    =   { 1,  1,  1,        identityNud,  identityLed},
  [ADD]         =   {50, 30, 30,          prefixNud,    infixLedL},
  [MINUS]       =   {50, 30, 30,          prefixNud,    infixLedL},
  [MULTIPLY]    =   {50, 40, 40,          prefixNud,    infixLedL},
  [DIVIDE]      =   { 0, 40, 40,           errorNud,    infixLedL},
  [MODULUS]     =   { 0, 40, 40,           errorNud,    infixLedL},
  [TILDE]       =   {50,  0,100,          prefixNud,     errorLed},
  [NOT]         =   {50,  0,100,          prefixNud,     errorLed},
  [INC]         =   {50,  0, 60,          prefixNud,    suffixLed},
  [DEC]         =   {50,  0, 60,          prefixNud,    suffixLed},
  /*
  [OPEN_PARENT] =   {50,  0, 60,          parentNud, errorParentLed},
  [CLOSE_PARENT]=   { 0,  0,  0,     errorParentNud,         NULL},
  */
  [EOL]         =   { 0,  0,  0,  missingOperandNud,         NULL},
};

Tokenizer *tokenizer;

//handles prefix
//unary, inc, dec etc
Symbol *prefixNud(Symbol *symbol) {
  symbol->arity = PREFIX;
  symbol->right = expression(getPrefixRBP(symbol));
  return symbol;
}

//handles infix
//called when operator binds to the left
//for left associativity
Symbol *infixLedL(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->left = left;
  symbol->right = expression(getInfixRBP(symbol));
  return symbol;
}

//for right associativity
Symbol *infixLedR(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->left = left;
  symbol->right = expression(getInfixRBP(symbol) - 1);
  return symbol;
}

//postfix, eg: "++", "--"
Symbol *suffixLed(Symbol *symbol, Symbol *left) {
  /*
  if(!(isIdentifierToken(left->token)))
    throwException(ERR_SYNTAX, token, 0,
    "Does not expect suffix %s here", token->str);
  */
  symbol->arity = SUFFIX;
  symbol->left = left;
  symbol->right = NULL;
  return symbol;
}
/*
Symbol *parentNud() {
  Symbol *left = expression(0);
  if(
  
}
*/

//just returns the symbol (numbers, var)
Symbol *identityNud(Symbol *symbol) {
  symbol->arity = IDENTITY;
  return symbol;
}

//error handling for illegal prefix
Symbol *errorNud(Symbol *symbol) {
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Operator %s is not a unary operator", symbol->token->str);
}

//error handling for missing right operand
Symbol *missingOperandNud(Symbol *symbol) {
  throwException(ERR_MISSING_OPERAND, symbol->token, 0,
  "Expected an operand here, but none received", symbol->token->str);
}

//error handling for illegal infix
Symbol *errorLed(Symbol *symbol, Symbol *left) {
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Operator %s is not a binary operator", symbol->token->str);
}

//error handling for numbers and variables 
//they cannot be infix, thus if led is called an error is thrown
Symbol *identityLed(Symbol *symbol, Symbol *left) {
  throwException(ERR_EXPECTED_OPERATOR, symbol->token, 0,
  "Expected an operator here, but received %s instead", symbol->token->str);
}

//main parser
Symbol *expression(int rbp) {
  Symbol *left, *symbol;
  symbol = getSymbol(tokenizer);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP(peekSymbol(tokenizer))) {
    symbol = getSymbol(tokenizer);
    left = ledOf(symbol)(symbol, left);
  }
  return left;
}