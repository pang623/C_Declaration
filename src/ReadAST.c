#include "ReadAST.h"
#include <stdio.h>

ReadFunction readSymbolTable[] = {
  [IDENTIFIER]          =   identifierRead,
  [NUMBER]              =      generalRead,
  [OPEN_SQR]            =        arrayRead,
  [FUNCTION]            =     functionRead,
  [POINTER]             =      pointerRead,
  [COMMA]               =      generalRead,
  [TYPE]                =      generalRead,
  [PLUS_SIGN]           =      generalRead,
  [MINUS_SIGN]          =      generalRead,
  [ADD]                 =      generalRead,
  [SUBTRACT]            =      generalRead,
  [MULTIPLY]            =      generalRead,
  [DIVIDE]              =      generalRead,
  [MODULUS]             =      generalRead,
  [BIT_AND]             =      generalRead,
  [BIT_XOR]             =      generalRead,
  [BIT_OR]              =      generalRead,
  [BIT_NOT]             =      generalRead,
  [L_SHIFT]             =      generalRead,
  [R_SHIFT]             =      generalRead,
  [LOGI_NOT]            =      generalRead,
  [LOGI_AND]            =      generalRead,
  [LOGI_OR]             =      generalRead,
  [LESSER]              =      generalRead,
  [GREATER]             =      generalRead,
  [LESS_OR_EQUAL]       =      generalRead,
  [GREATER_OR_EQUAL]    =      generalRead,
  [EQUALITY]            =      generalRead,
  [NOT_EQUAL]           =      generalRead,
  [OPEN_PARENT]         =       ignoreRead,
};

int readRight = 0;
int readArray = 0;

char *functionRead(Symbol *symbol) {
  char *str, *newStr;
  //if no function parameters
  if(symbol->child[1] == NULL)
    str = createString("function ");
  else
    str = createString("function taking in ");
  newStr = concat(str, createString(symbol->token->str));
  newStr = readAST(symbol->child[1], newStr);
  readRight = 1;
  return concat(newStr, createString(") returning "));
}

char *pointerRead(Symbol *symbol) {
  char *str = createString("pointer to ");
  return str;
}

char *identifierRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  if(readArray)
    return str;
  return concat(str, createString(" is "));
}

char *generalRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  return str;
}

char *ignoreRead(Symbol *symbol) {
  return createString("");
}

char *arrayRead(Symbol *symbol) {
  int result;
  char *str = createString("array of ");
  if(symbol->child[1] == NULL)
    return str;
  else if(isExpressionReducible(symbol->child[1])) {
    result = integerEvaluate(symbol->child[1]);
    str = concat(str, convertIntToStr(result));
  }else {
    readArray = 1;
    str = readAST(symbol->child[1], str);
  }
  readRight = 1;
  readArray = 0;
  return concat(str, createString(" of "));
}

char *readAST(Symbol *AST, char *str) {
  if(AST == NULL)
    return str;
  str = readAST(AST->child[0], str);
  str = concat(str, readSymbol(AST)(AST));
  if(!readRight)
    str = readAST(AST->child[1], str);
  readRight = 0;
  return str;
}
