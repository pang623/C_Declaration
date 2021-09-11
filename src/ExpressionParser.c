#include "ExpressionParser.h"
#include <stdio.h>
#include <stdlib.h>

//If both nud and led are NULL, it indicates that that particular symbol is not supported
SymbolAttrTable expressionSymbolTable[LAST_SYMBOL] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,   nud,             led}
  [NUMBER]             =   { NIL,  NIL,  NIL,        identityNud,     identityLed},
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,        identityNud,     identityLed},
  //Arithmetic
  [ADD]                =   { 140,  120,  120,          prefixNud,       infixLedL},
  [SUBTRACT]           =   { 140,  120,  120,          prefixNud,       infixLedL},
  [MULTIPLY]           =   { NIL,  130,  130,           errorNud,       infixLedL},
  [DIVIDE]             =   { NIL,  130,  130,           errorNud,       infixLedL},
  [MODULUS]            =   { NIL,  130,  130,           errorNud,       infixLedL},
  [INC_BEFORE]         =   { 140,  NIL,  150,    prefixIncDecNud, suffixIncDecLed},
  [DEC_BEFORE]         =   { 140,  NIL,  150,    prefixIncDecNud, suffixIncDecLed},
  //Bitwise
  [BIT_AND]            =   { NIL,   80,   80,           errorNud,       infixLedL},
  [BIT_XOR]            =   { NIL,   70,   70,           errorNud,       infixLedL},
  [BIT_OR]             =   { NIL,   60,   60,           errorNud,       infixLedL},
  [BIT_NOT]            =   { 140,  NIL,  NIL,          prefixNud,        errorLed},
  [L_SHIFT]            =   { NIL,  110,  110,           errorNud,       infixLedL},
  [R_SHIFT]            =   { NIL,  110,  110,           errorNud,       infixLedL},
  //Logical
  [LOGI_NOT]           =   { 140,  NIL,  NIL,          prefixNud,        errorLed},
  [LOGI_AND]           =   { NIL,   50,   50,           errorNud,       infixLedL},
  [LOGI_OR]            =   { NIL,   40,   40,           errorNud,       infixLedL},
  //Relational
  [LESSER]             =   { NIL,  100,  100,           errorNud,       infixLedL},
  [GREATER]            =   { NIL,  100,  100,           errorNud,       infixLedL},
  [LESS_OR_EQUAL]      =   { NIL,  100,  100,           errorNud,       infixLedL},
  [GREATER_OR_EQUAL]   =   { NIL,  100,  100,           errorNud,       infixLedL},
  [EQUALITY]           =   { NIL,   90,   90,           errorNud,       infixLedL},
  [NOT_EQUAL]          =   { NIL,   90,   90,           errorNud,       infixLedL},
  //Assignment
  [ASSIGNMENT]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [ADD_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [SUBT_ASSIGN]        =   { NIL,   20,   20,           errorNud,       infixLedR},
  [MUL_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [DIV_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [MOD_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [AND_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [OR_ASSIGN]          =   { NIL,   20,   20,           errorNud,       infixLedR},
  [XOR_ASSIGN]         =   { NIL,   20,   20,           errorNud,       infixLedR},
  [L_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,       infixLedR},
  [R_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,       infixLedR},
  //Misc.
  [OPEN_PARENT]        =   {  0,   NIL,  NIL,          parentNud,        errorLed},
  [CLOSE_PARENT]       =   {  0,     0,    0,           errorNud,            NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,           errorNud,   sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,           errorNud,            NULL},
  [EOL]                =   {  0,     0,    0,  missingOperandNud,            NULL},
  [COMMA]              =   {  0,     0,    0,               NULL,            NULL},
};

//handles prefix
//unary, inc, dec etc
Symbol *prefixNud(Symbol *symbol, Symbol *left) {
  symbol->arity = PREFIX;
  symbol->child[0] = expression(getPrefixRBP(symbol));
  if(symbol->id == ADD)
    symbol->id = PLUS_SIGN;
  if(symbol->id == SUBTRACT)
    symbol->id = MINUS_SIGN;
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

//for right associativity (assignment operators)
Symbol *infixLedR(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  if(left->id == NUMBER) {
    freeSymbol(symbol);
    throwException(ERR_SYNTAX, left->token, 0,
    "Cannot assign expression to numbers");
  }
  symbol->child[0] = left;
  symbol->child[1] = expression(getInfixRBP(symbol) - 1);
  return symbol;
}

Symbol *sqrBracketLed(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  if(peekSymbol(symbolParser)->id != CLOSE_SQR)
    symbol->child[1] = expression(0);
  //note: cannot have -ve value and floating point value for array size
  verifyIsNextSymbolThenConsume(CLOSE_SQR, "]");
  return symbol;
}

//prefix "++" "--"
Symbol *prefixIncDecNud(Symbol *symbol, Symbol *left) {
  Symbol *right = getSymbol(symbolParser);
  if(right->id != IDENTIFIER) {
    freeSymbol(symbol);
    throwException(ERR_SYNTAX, right->token, 0,
    "Operand must be a variable", right->token->str);
  }
  symbol->arity = PREFIX;
  symbol->child[0] = right;
  return symbol;
}

//postfix "++" "--"
Symbol *suffixIncDecLed(Symbol *symbol, Symbol *left) {
  if(left->id != IDENTIFIER) {
    freeSymbol(symbol);
    throwException(ERR_SYNTAX, left->token, 0,
    "Operand must be a variable", left->token->str);
  }
  symbol->arity = SUFFIX;
  symbol->id += 1;
  symbol->child[0] = left;
  return symbol;
}

Symbol *parentNud(Symbol *symbol, Symbol *left) {
  symbol->child[0] = expression(0);
  verifyIsNextSymbolThenConsume(CLOSE_PARENT, ")");
  return symbol;
}

//just returns the symbol (numbers, var)
Symbol *identityNud(Symbol *symbol, Symbol *left) {
  int i;
  int *type = &i;
  if(isSymbolKeyword(symbol, 0)) {
    freeSymbol(left);
    throwException(ERR_ILLEGAL_KEYWORD_USAGE, symbol->token, 0,
    "Keyword %s cannot be used here", symbol->token->str);
  }
  symbol->arity = IDENTITY;
  return symbol;
}

//error handling for illegal prefix
Symbol *errorNud(Symbol *symbol, Symbol *left) {
  freeSymbol(left);
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Operator %s is not a unary operator", symbol->token->str);
  return NULL;
}

//error handling for missing right operand
Symbol *missingOperandNud(Symbol *symbol, Symbol *left) {
  freeSymbol(left);
  throwException(ERR_MISSING_OPERAND, symbol->token, 0,
  "Expected an operand here, but none received", symbol->token->str);
  return NULL;
}

//error handling for illegal infix
Symbol *errorLed(Symbol *symbol, Symbol *left) {
  freeSymbol(left);
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Operator %s is not a binary operator", symbol->token->str);
  return NULL;
}

//error handling for numbers and variables 
//they cannot be infix, thus if led is called an error is thrown
Symbol *identityLed(Symbol *symbol, Symbol *left) {
  freeSymbol(left);
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Identifiers and numbers cannot be used here, but received %s here", symbol->token->str);
  return NULL;
}

Symbol *expression(int rbp) {
  return tdop(rbp, expressionSymbolTable);
}