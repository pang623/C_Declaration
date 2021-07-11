#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

//handles binary only at the moment
BindingPower symbolTable[5] = {
  //left associativity, rbp > lbp
  //right associativity, rbp < lbp
  {"+", 10,  9},
  {"-", 10,  9},
  {"*", 20, 19},
  {"/", 20, 19},
  {"%", 20, 19},
};

Tokenizer *tokenizer;

//returns left/right binding power of symbol by reading the symbol table
int getBP(int bp_type, Token *token) {
  
}

//handles prefix
Symbol *nud() {
  
}

//handles infix
Symbol *led(Symbol *left) {
  
}

//main parser
Symbol *expression(int rbp) {
  
}