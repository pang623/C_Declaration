#include "ExpressionParser.h"
#include <stdio.h>
#include <stdlib.h>

SymbolAttrTable expressionSymbolTable[] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,     nud,     led}
  [NUMBER]             =   { NIL,  NIL,  NIL,        identityNud,  identityLed},
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,        identityNud,  identityLed},
  //Arithmetic
  [ADD]                =   { 140,  120,  120,          prefixNud,    infixLedL},
  [SUBTRACT]           =   { 140,  120,  120,          prefixNud,    infixLedL},
  [MULTIPLY]           =   { NIL,  130,  130,           errorNud,    infixLedL},
  [DIVIDE]             =   { NIL,  130,  130,           errorNud,    infixLedL},
  [MODULUS]            =   { NIL,  130,  130,           errorNud,    infixLedL},
  [INC_BEFORE]         =   { 140,  NIL,  150,          prefixNud,    suffixLed},
  [DEC_BEFORE]         =   { 140,  NIL,  150,          prefixNud,    suffixLed},
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
  [ADD_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [SUBT_ASSIGN]        =   { NIL,   20,   20,           errorNud,    infixLedR},
  [MUL_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [DIV_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [MOD_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [AND_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [OR_ASSIGN]          =   { NIL,   20,   20,           errorNud,    infixLedR},
  [XOR_ASSIGN]         =   { NIL,   20,   20,           errorNud,    infixLedR},
  [L_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,    infixLedR},
  [R_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,    infixLedR},
  //Misc.
  [OPEN_PARENT]        =   {  0,   NIL,  NIL,          parentNud,     errorLed},
  [CLOSE_PARENT]       =   {  0,     0,    0,           errorNud,         NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,              errorNud,  sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,  NULL},
  [EOL]                =   {  0,     0,    0,  missingOperandNud,         NULL},
};

StatementKeywordTable expKeywordTable[] = {
  {"int"        , TYPE,    NULL},
  {"char"       , TYPE,    NULL},
  {"float"      , TYPE,    NULL},
  {"double"     , TYPE,    NULL},
  {"if"         , FLOW,    NULL},
  {"else"       , FLOW,    NULL},
  {"while"      , FLOW,    NULL},
  {"for"        , FLOW,    NULL},
  {"do"         , FLOW,    NULL},
  {"switch"     , FLOW,    NULL},
  {"case"       , FLOW,    NULL},
  {"continue"   , FLOW,    NULL},
  {"break"      , FLOW,    NULL},
  {NULL         , TYPE,    NULL},
};

//handles prefix
//unary, inc, dec etc
Symbol *prefixNud(Symbol *symbol) {
  symbol->arity = PREFIX;
  symbol->child[0] = expression(getPrefixRBP(symbol));
  if(symbol->id == ADD)
    symbol->id = PLUS_SIGN;
  if(symbol->id == SUBTRACT)
    symbol->id = MINUS_SIGN;
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

Symbol *sqrBracketLed(Symbol *symbol, Symbol *left) {
  if(!(isIdentifierToken(left->token)))
    throwException(ERR_WRONG_SYMBOL, left->token, 0,
    "Array name should be declared with identifier, %s is not identifier", left->token->str);
  symbol->arity = INFIX;
  symbol->child[0] = left;
  symbol->child[1] = expression(0);
  verifyIsNextSymbolThenConsume(symbolParser->tokenizer, CLOSE_SQR, "]");
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
  symbol->id += 1;
  symbol->child[0] = left;
  freeSymbol(symbol->child[1]);
  return symbol;
}

Symbol *parentNud(Symbol *symbol) {
  symbol->child[0] = expression(0);
  verifyIsNextSymbolThenConsume(symbolParser->tokenizer, CLOSE_PARENT, ")");
  freeSymbol(symbol->child[1]);
  return symbol;
}

int isSymbolKeywordType(Symbol *symbol, int keywordType, int *index) {
  if(!isIdentifierToken(symbol->token))
    return 0;
  int i = 0;
  while(strcmp(symbol->token->str, expKeywordTable[i].keyword)) {
    i++;
    if(expKeywordTable[i].keyword == NULL && keywordType == TYPE) {
      *index = i;
      return 1;
    }
    else
      return 0;
  }
  if(keywordType != ALL && expKeywordTable[i].type != keywordType)
    return 0;
  else {
    *index = i;
    return 1;
  }
}

//just returns the symbol (numbers, var)
Symbol *identityNud(Symbol *symbol) {
  int temp = 0;
  int *index = &temp;
  if(isSymbolKeywordType(symbol, ALL, index))
    throwException(ERR_ILLEGAL_KEYWORD_USAGE, symbol->token, 0,
    "Keyword %s cannot be used here", symbol->token->str);
  symbol->arity = IDENTITY;
  freeSymbol(symbol->child[0]);
  freeSymbol(symbol->child[1]);
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
  setSymbolTable(symbolParser, expressionSymbolTable);
  symbol = getSymbol(symbolParser);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP(peekSymbol(symbolParser->tokenizer))) {
    symbol = getSymbol(symbolParser);
    left = ledOf(symbol)(symbol, left);
  }
  return left;
}

Symbol *parse(int rbp) {
  Symbol *left = expression(rbp);
  verifyIsNextSymbolThenConsume(symbolParser->tokenizer, EOL, ";");
  return left;
}