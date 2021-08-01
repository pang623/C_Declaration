#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable symbolTable[] = {
  //[SYMBOL]    =   {prefixRBP, infixRBP, infixLBP,     nud,     led}
  [NUMBER]      =   {NIL, NIL, NIL,        identityNud,  identityLed},
  [VARIABLE]    =   {NIL, NIL, NIL,        identityNud,  identityLed},
  [ADD]         =   { 50,  30,  30,          prefixNud,    infixLedL},
  [MINUS]       =   { 50,  30,  30,          prefixNud,    infixLedL},
  [MULTIPLY]    =   { 50,  40,  40,          prefixNud,    infixLedL},
  [DIVIDE]      =   {NIL,  40,  40,           errorNud,    infixLedL},
  [MODULUS]     =   {NIL,  40,  40,           errorNud,    infixLedL},
  [TILDE]       =   { 50, NIL, NIL,          prefixNud,     errorLed},
  [NOT]         =   { 50, NIL, NIL,          prefixNud,     errorLed},
  [INC]         =   { 50, NIL,  60,          prefixNud,    suffixLed},
  [DEC]         =   { 50, NIL,  60,          prefixNud,    suffixLed},
  [OPEN_PARENT] =   {  0, NIL, NIL,          parentNud,     errorLed},
  [CLOSE_PARENT]=   {  0,   0,   0,           errorNud,     errorLed},
  [EOL]         =   {  0,   0,   0,  missingOperandNud,         NULL},
};

Tokenizer *tokenizer;

//handles prefix
//unary, inc, dec etc
Symbol *prefixNud(Symbol *symbol) {
  symbol->arity = PREFIX;
  symbol->child[0] = expression(getPrefixRBP(symbol));
  return symbol;
}

//handles infix
//called when operator binds to the left
//for left associativity
Symbol *infixLedL(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  symbol->child[1] = expression(getInfixRBP(symbol));
  return symbol;
}

//for right associativity
Symbol *infixLedR(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  symbol->child[1] = expression(getInfixRBP(symbol) - 1);
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
  symbol->child[0] = left;
  return symbol;
}

Symbol *parentNud(Symbol *symbol) {
  Symbol *left = symbol;
  left->child[0] = expression(0);
  Symbol *_symbol = getSymbol(tokenizer);
  verifyIsSymbolThenConsume(")", _symbol);
  return left;
}

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