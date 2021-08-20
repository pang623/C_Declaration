#include "CDecl.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable CDeclSymbolTable[] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,     nud,     led}
  [NUMBER]             =   { NIL,  NIL,  NIL,           identityNud,  identityLed},
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,           identityNud,  identityLed},
  //Arithmetic
  [MULTIPLY]           =   { NIL,  130,  130,             pointerNud,    errorLed},
  //Bitwise
  //Misc.
  [OPEN_PARENT]        =   {  0,   NIL,  NIL,             parentNud,     errorLed},
  [CLOSE_PARENT]       =   {  0,     0,    0,              errorNud,         NULL},
  [OPEN_SQR]           =   {NIL,     0,  170,              errorNud,  sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,  sqrBracketLed},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,         NULL},
};

StatementKeywordTable keywordTable[] = {
  {"int"    , TYPE, typeFud},
  {"char"   , typeFud},
  {"float"  , typeFud},
  {"double" , typeFud},
    {"if",  FLOW, 
};

Tokenizer *tokenizer;

//handles prefix
//unary, inc, dec etc
Symbol *prefixNud(Symbol *symbol) {
  symbol->arity = PREFIX;
  symbol->child[0] = expression(getPrefixRBP(symbol));
  freeSymbol(symbol->child[1]);
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
  //if suffixLed is called, meaning it is not xxx_BEFORE, but is xxx_AFTER instead
  symbol->child[0] = left;
  freeSymbol(symbol->child[1]);
  return symbol;
}

Symbol *parentNud(Symbol *symbol) {
  Symbol *left = symbol;
  left->child[0] = expression(0);
  Symbol *_symbol = peekSymbol(tokenizer);
  verifyIsNextSymbolThenConsume(tokenizer, CLOSE_PARENT, ")");
  freeSymbol(left->child[1]);
  return left;
}

//just returns the symbol (numbers, var)
Symbol *identityNud(Symbol *symbol) {
  /*
  if(isSymbolKeyword(symbol))
    throwException(ERR_ILLEGAL_IDENTIFIER, symbol->token, 0,
    "Keyword '%s' cannot be used here", symbol->token->str);
  else {*/
    symbol->arity = IDENTITY;
    freeSymbol(symbol->child[0]);
    freeSymbol(symbol->child[1]);
    return symbol;
  //}
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
/*
Symbol *parse(int rbp) {
  Symbol *left = expression(rbp);
  verifyIsNextSymbolThenConsume(tokenizer, EOL, ";");
  return left;
}
Symbol
int A
Symbol *statement() {
  Symbol *symbol;
  if(isSymbolKeyword(peekSymbol(tokenizer))) {
    symbol = getSymbol(tokenizer);
    return fudOf(symbol)(symbol);
  }
  Symbol *left = expression(0);
  verifyIsNextSymbolThenConsume(tokenizer, EOL, ";");
  return left;
}
*/
Symbol *statement() {
  Symbol *left;
  Symbol *symbol = getSymbol(tokenizer);
  if(symbol->id != IDENTIFIER)
    throwException(ERR_TYPE_NAME, symbol->token, 0,
    "Type must be an identifier", symbol->token->str);
  else
    left = fudOf(symbol)(symbol);
  verifyIsNextSymbolThenConsume(tokenizer, EOL, ";");
  return left;
}

Symbol *typeFud(Symbol *symbol) {
  Symbol *left = symbol;
  left->id = TYPE;
  left->child[0] = cdecl(0);
  return left;
}

void verifyHasNoIdentifier(Symbol *symbol) {
  if(symbol == NULL)
    return;
  if(symbol->id == IDENTIFIER)
    throwException(ERR_ARRAY_SUBSCRIPT, symbol->token, 0,
    "Variable cannot be used in array subscripting", symbol->token->str);
  verifyHasNoIdentifier(symbol->child[0]);
  verifyHasNoIdentifier(symbol->child[1]);
}

Symbol *sqrBracketLed(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  symbol->child[1] = expression(getInfixRBP(symbol));
  verifyHasNoIdentifier(symbol->child[1]);
  verifyIsNextSymbolThenConsume(tokenizer, CLOSE_SQR, "]");
  return symbol;
}