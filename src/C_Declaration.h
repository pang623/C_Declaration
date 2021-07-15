#ifndef C_DECLARATION_H
#define C_DECLARATION_H

#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CDecl_Errors.h"

//bp_type
#define   LEFTBP    0
#define   RIGHTBP   1

typedef struct Symbol Symbol;
typedef struct BindingPower BindingPower;

struct Symbol {
  Symbol *left;
  Symbol *right;
  Token *token;
};

struct BindingPower {
  char *symbol;
  int rbp;
  int lbp;
};

Symbol *expression(int rbp);
Symbol *nud();
Symbol *led(Symbol *left);
int bp(int bp_type, Token *token);

#endif // C_DECLARATION_H
