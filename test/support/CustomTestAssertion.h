#ifndef CUSTOMTESTASSERTION_H
#define CUSTOMTESTASSERTION_H

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "Arity.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"

#define   isOperatorNotMatch(testChild, child)   \
            (child->token->type == TOKEN_OPERATOR_TYPE && (strcmp(((TestOperator *)testChild)->_operator, child->token->str)))
            
#define   isNumberNotMatch(testChild, child)   \
            ((child->token->type == TOKEN_INTEGER_TYPE || child->token->type == TOKEN_FLOAT_TYPE) && \
            (strcmp(((TestNumber *)testChild)->value, child->token->str)))

#define   isIdentifierNotMatch(testChild, child)   \
            (child->token->type == TOKEN_IDENTIFIER_TYPE && (strcmp(((TestIdentifier *)testChild)->str, child->token->str)))

//TestSymbol Types
#define   OPERATOR_TYPE       10
#define   NUMBER_TYPE         11
#define   IDENTIFIER_TYPE     12

//TEST_ASSERT MACRO
#define   TEST_ASSERT_SYMBOL(expectedSymbolId, expectedSymbolChar, childZero, childOne, root)      \
            testAssertSymbol(expectedSymbolId, expectedSymbolChar, childZero, childOne, root, __LINE__)

typedef struct TestSymbolType TestSymbolType;
typedef struct TestOperator TestOperator;
typedef struct TestIdentifier TestIdentifier;
typedef struct TestNumber TestNumber;

struct TestSymbolType {
  int type;
};

struct TestOperator {
  int type;
  char *_operator;
};

struct TestIdentifier {
  int type;
  char *str;
};

struct TestNumber {
  int type;
  char *value;
};

char *createMessage(char *message, ...);
void testAssertSymbol(int symbolId, char *symbol, TestSymbolType *childZero, TestSymbolType *childOne, Symbol *root, int lineNum);
TestSymbolType *Number(char *value);
TestSymbolType *Identifier(char *str);
TestSymbolType *Operator(char *_operator);

#endif // CUSTOMTESTASSERTION_H
