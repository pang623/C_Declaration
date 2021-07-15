#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

//handles binary only at the moment
BindingPower symbolTable[6] = {
  //left associativity, rbp > lbp
  //right associativity, rbp < lbp
  {"+", 10,  9},
  {"-", 10,  9},
  {"*", 20, 19},
  {"/", 20, 19},
  {"%", 20, 19},
  {NULL, 0,  0},
};

Tokenizer *tokenizer;

//returns left/right binding power of symbol by reading the symbol table
int getBP(int bp_type, Token *token) {
  int i = 0;
  if(token->type == TOKEN_NULL_TYPE)
    return 0;
  while(stricmp(token->str, symbolTable[i].symbol))
    if(symbolTable[i].symbol == NULL)
      throwException(ERR_INVALID_SYMBOL, token, 0,
      "An invalid symbol '%s' is inputted", token->str);
    else
      i++;
  if(bp_type == LEFTBP)
    return symbolTable[i].lbp;
  else if(bp_type == RIGHTBP)
    return symbolTable[i].rbp;
}

//handles prefix
Symbol *nud() {
  
}

//handles infix
Symbol *led(Symbol *left) {
  
}
/*
//main parser
Symbol *expression(int rbp) {
  Symbol *left;
  left = nud();
  while(rbp < getBP(LEFTBP, peek(tokenizer)))
    left = led(left);
  return left;
}*/