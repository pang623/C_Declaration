#include "ExpressionParser.h"
#include <stdio.h>
#include <stdlib.h>

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
  [ASSIGNMENT]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [ADD_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [SUBT_ASSIGN]        =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [MUL_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [DIV_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [MOD_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [AND_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [OR_ASSIGN]          =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [XOR_ASSIGN]         =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [L_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  [R_SHIFT_ASSIGN]     =   { NIL,   20,   20,           errorNud,   infixAssgnLed},
  //Misc.
  [OPEN_PARENT]        =   {  0,   NIL,  NIL,          parentNud,        errorLed},
  [CLOSE_PARENT]       =   {  0,     0,    0,           errorNud,            NULL},
  [OPEN_SQR]           =   {NIL,     0,  150,           errorNud,   sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,           errorNud,            NULL},
  [EOL]                =   {  0,     0,    0,  missingOperandNud,            NULL},
};

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

int isExpressionHasFloatNum(Symbol *symbol) {
  if(symbol == NULL)
    return 0;
  if(symbol->token->type == TOKEN_FLOAT_TYPE)
    return 1;
  int result1 = isExpressionHasFloatNum(symbol->child[0]);
  if(result1 == 1)
    return result1;
  int result2 = isExpressionHasFloatNum(symbol->child[1]);
  return result2;
}

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
Symbol *infixAssgnLed(Symbol *symbol, Symbol *left) {
  Symbol *symbolCheck = left;
  symbol->arity = INFIX;
  //skips open parent (grouping)
  while(symbolCheck->id == OPEN_PARENT)
    symbolCheck = symbolCheck->child[0];
  //check is the left operand is rvalue
  if(symbolCheck->id != IDENTIFIER && symbolCheck->id != OPEN_SQR) {
    freeSymbol(symbol);
    throwException(ERR_SYNTAX, symbolCheck->token, 0,
    "Cannot assign expression to rvalue");
  }
  symbol->child[0] = left;
  symbol->child[1] = expression(getInfixRBP(symbol) - 1);
  return symbol;
}

Symbol *sqrBracketLed(Symbol *symbol, Symbol *left) {
  symbol->arity = INFIX;
  symbol->child[0] = left;
  if(peekSymbol(symbolParser)->id != CLOSE_SQR) {
    symbol->child[1] = expression(0);
    //cannot have -ve value and floating point value for array size
    if(isExpressionHasFloatNum(symbol->child[1]))
      throwException(ERR_ARRAY_SIZE_FLOATING_NUM, symbol->child[1]->token, 0,
      "Array size cannot have floating point values");
    if(isExpressionReducible(symbol->child[1]))
      if(integerEvaluate(symbol->child[1]) < 0)
        throwException(ERR_ARRAY_SIZE_NEGATIVE, symbol->child[1]->token, 0,
        "Array size cannot be negative");
  }
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
  "Expected an operand here, but none received");
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
  "%s cannot be used here", symbol->token->str);
  return NULL;
}

Symbol *expression(int rbp) {
  return tdop(rbp, expressionSymbolTable);
}