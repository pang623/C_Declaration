#include "ReadAST.h"
#include <stdio.h>

createPrefixFunction(prefixPlus, +);
createPrefixFunction(prefixMinus, -);

createInfixFunction(infixAdd, +);
createInfixFunction(infixSubtract, -);
createInfixFunction(infixMultiply, *);
createInfixFunction(infixDivide, /);
createInfixFunction(infixModulus, %);

createInfixFunction(infixBitwiseAnd, &);
createInfixFunction(infixBitwiseXor, ^);
createInfixFunction(infixBitwiseOr, |);
createPrefixFunction(prefixBitwiseNot, ~);
createInfixFunction(infixLeftShift, <<);
createInfixFunction(infixRightShift, >>);

createPrefixFunction(prefixLogicalNot, !);
createInfixFunction(infixLogicalAnd, &&);
createInfixFunction(infixLogicalOr, ||);

createInfixFunction(infixLesser, <);
createInfixFunction(infixGreater, >);
createInfixFunction(infixLesserEq, <=);
createInfixFunction(infixGreaterEq, >=);
createInfixFunction(infixEquality, ==);
createInfixFunction(infixNotEqual, !=);


EvaluateFunction evaluateSymbolTable[] = {
  [PLUS_SIGN]          =          prefixPlus,
  [MINUS_SIGN]         =         prefixMinus,
  //Arithmetic
  [ADD]                =            infixAdd,
  [SUBTRACT]           =       infixSubtract,
  [MULTIPLY]           =       infixMultiply,
  [DIVIDE]             =         infixDivide,
  [MODULUS]            =        infixModulus,
  //Bitwise
  [BIT_AND]            =     infixBitwiseAnd,
  [BIT_XOR]            =     infixBitwiseXor,
  [BIT_OR]             =      infixBitwiseOr,
  [BIT_NOT]            =    prefixBitwiseNot,
  [L_SHIFT]            =      infixLeftShift,
  [R_SHIFT]            =     infixRightShift,
  //Logical
  [LOGI_NOT]           =    prefixLogicalNot,
  [LOGI_AND]           =     infixLogicalAnd,
  [LOGI_OR]            =      infixLogicalOr,
  //Relational
  [LESSER]             =         infixLesser,
  [GREATER]            =        infixGreater,
  [LESS_OR_EQUAL]      =       infixLesserEq,
  [GREATER_OR_EQUAL]   =      infixGreaterEq,
  [EQUALITY]           =       infixEquality,
  [NOT_EQUAL]          =       infixNotEqual,
};

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

char *convertIntToStr(int num) {
  int len = snprintf(NULL, 0, "%d", num);
  char *resultStr = malloc(len + 1);
  snprintf(resultStr, len + 1, "%d", num);
  return resultStr;
}

int isExpressionReducible(Symbol *symbol) {
  if(symbol == NULL)
    return 1;
  if(symbol->id == IDENTIFIER)
    return 0;
  int result1 = isExpressionReducible(symbol->child[0]);
  if(result1 == 0)
    return result1;
  int result2 = isExpressionReducible(symbol->child[1]);
  return result2;
}

int integerEvaluate(Symbol *symbol) {
  if(symbol == NULL)
    return 0;
  if(symbol->id == OPEN_PARENT)
    return integerEvaluate(symbol->child[0]);
  if(symbol->child[0] == NULL && symbol->child[1] == NULL)
    return getSymbolInteger(symbol);
  int left_val = integerEvaluate(symbol->child[0]);
  int right_val = integerEvaluate(symbol->child[1]);
  
  return evaluate(symbol)(left_val, right_val);
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

/*
//put to another module
char *translate(char *cDecl) {
  Tokenizer *tokenizer = createTokenizer(cDecl);
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  char *newStr = readAST(AST, createString(""));
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
  return newStr;
}*/