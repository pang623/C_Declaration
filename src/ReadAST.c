#include "ReadAST.h"

ReadFunction readSymbolTable[] = {
  [IDENTIFIER]      = identifierRead,//" is ",
  [NUMBER]          = generalRead,//" of ",
  [OPEN_SQR]        = arrayRead,//"array of ",
  [FUNCTION]        = functionRead,//"function taking in",
  [POINTER]         = pointerRead,//"pointer to ",
  [COMMA]           = generalRead,//"",
  [TYPE]            = generalRead,//"",
  [OPEN_PARENT]     = ignoreRead,//"",
};

char *functionRead(Symbol *symbol) {
  char *str;
  //if no function parameters
  if(symbol->child[1] == NULL)
    str = createString("function ");
  else
    str = createString("function taking in ");
  concat(str, createString(symbol->token->str));
  str = readAST(symbol->child[1], str);
  readRight = 1;
  return concat(str, createString(") returning "));
}

char *pointerRead(Symbol *symbol) {
  char *str = createString("pointer to ");
  return str;
}

char *identifierRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  concat(str, createString(" is "));
  return str;
}

char *generalRead(Symbol *symbol) {
  char *str = createString(symbol->token->str);
  return str;
}

char *ignoreRead(Symbol *symbol) {
  return createString("");
}

char *arrayRead(Symbol *symbol) {
  char *str = createString("array of ");
  str = readAST(symbol->child[1], str);
  readRight = 1;
  concat(str, createString(" of "));
  return str;
}

char *concat(char *s1, char *s2) {
  char *result = realloc(s1, strlen(s1) + strlen(s2) + 1);
  strcat(result, s2);
  return result;
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


//put to another module
char *translate(char *cDecl) {
  Tokenizer *tokenizer = createTokenizer(cDecl);
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  char *newStr = readAST(AST, createString(""));
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
  return newStr;
}