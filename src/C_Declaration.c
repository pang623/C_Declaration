#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable symbolTable[] = {
  //[SYMBOLID]           =   {prefixRBP, infixRBP, infixLBP,     nud,     led}
  [NUMBER]             =   { NIL,  NIL,  NIL,        identityNud,  identityLed},
  [VARIABLE]           =   { NIL,  NIL,  NIL,        identityNud,  identityLed},
  //Arithmetic
  [ADD]                =   { 140,  120,  120,          prefixNud,    infixLedL},
  [MINUS]              =   { 140,  120,  120,          prefixNud,    infixLedL},
  [MULTIPLY]           =   { NIL,  130,  130,          prefixNud,    infixLedL},
  [DIVIDE]             =   { NIL,  130,  130,           errorNud,    infixLedL},
  [MODULUS]            =   { NIL,  130,  130,           errorNud,    infixLedL},
  [INC]                =   { 140,  NIL,  150,          prefixNud,    suffixLed},
  [DEC]                =   { 140,  NIL,  150,          prefixNud,    suffixLed},
  //Bitwise
  [BIT_AND]            =   { NIL,   80,   80,           errorNud,    infixLedL},
  [BIT_XOR]            =   { NIL,   70,   70,           errorNud,    infixLedL},
  [BIT_OR]             =   { NIL,   60,   60,           errorNud,    infixLedL},
  [BIT_NOT]            =   { 140,  NIL,  NIL,          prefixNud,     errorLed},
  [L_SHIFT]            =   { NIL,  110,  110,           errorNud,    infixLedL},
  [R_SHIFT]            =   { NIL,  110,  110,           errorNud,    infixLedL},
  //Logical
  [LOGI_NOT]           =   { 140,  NIL,  NIL,          prefixNud,     errorLed},
  [LOGI_AND]           =   { NIL,   50,   50,           errorNud,    infixLedL},
  [LOGI_OR]            =   { NIL,   40,   40,           errorNud,    infixLedL},
  //Relational
  [LESSER]             =   { NIL,  100,  100,           errorNud,    infixLedL},
  [GREATER]            =   { NIL,  100,  100,           errorNud,    infixLedL},
  [LESS_OR_EQUAL]      =   { NIL,  100,  100,           errorNud,    infixLedL},
  [GREATER_OR_EQUAL]   =   { NIL,  100,  100,           errorNud,    infixLedL},
  [EQUALITY]           =   { NIL,   90,   90,           errorNud,    infixLedL},
  [NOT_EQUAL]          =   { NIL,   90,   90,           errorNud,    infixLedL},
  //Assignment
  [ASSIGNMENT]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_ADD]             =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_MINUS]           =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_MUL]             =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_DIV]             =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_MOD]             =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_BIT_AND]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_BIT_OR]          =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_BIT_XOR]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_L_SHIFT]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SH_R_SHIFT]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  //Misc.
  [OPEN_PARENT]        =   {  0,   NIL,  NIL,          parentNud,     errorLed},
  [CLOSE_PARENT]       =   {  0,     0,    0,           errorNud,     errorLed},
  [EOL]                =   {  0,     0,    0,  missingOperandNud,         NULL},
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
  Symbol *_symbol = peekSymbol(tokenizer);
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

void verifyExpressionFullyParsed(Tokenizer *tokenizer) {
  Symbol *symbol = getSymbol(tokenizer);
  if(isNULLToken(symbol->token) || isToken(";", symbol->token))
    freeSymbol(symbol);
  else
    throwException(ERR_PARSE_ERROR, symbol->token, 0,
    "An error has occured here, please check if there is missing pair of parentheses", symbol->token->str);
}

Symbol *parse(int rbp) {
  Symbol *left = expression(rbp);
  verifyExpressionFullyParsed(tokenizer);
  return left;
}