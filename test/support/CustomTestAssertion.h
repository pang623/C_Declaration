#ifndef CUSTOMTESTASSERTION_H
#define CUSTOMTESTASSERTION_H

#include "unity.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "Arity.h"
#include "Symbol.h"

//TestSymbol Types
#define   OPERATOR_TYPE       10
#define   NUMBER_TYPE         11
#define   IDENTIFIER_TYPE     12

//TEST_ASSERT MACROS
#define   TEST_ASSERT_INFIX(expectedSymbolId, expectedSymbolChar, childZero, childOne, root)      \
          testAssertInfix(expectedSymbolId, expectedSymbolChar, childZero, childOne, root)
#define   TEST_ASSERT_PREFIX(expectedSymbolId, expectedSymbolChar, childZero, root)               \
          testAssertSingleOperand(expectedSymbolId, expectedSymbolChar, PREFIX, childZero, root)
#define   TEST_ASSERT_SUFFIX(expectedSymbolId, expectedSymbolChar, childZero, root)               \
          testAssertSingleOperand(expectedSymbolId, expectedSymbolChar, SUFFIX, childZero, root)

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

void testAssertInfix(int symbolId, char *symbol, TestSymbolType *childZero, TestSymbolType *childOne, Symbol *root);
void testAssertSingleOperand(int symbolId, char *symbol, int arity, TestSymbolType *childZero, Symbol *root);
TestSymbolType *Number(char *value);
TestSymbolType *Identifier(char *str);
TestSymbolType *Operator(char *_operator);

#endif // CUSTOMTESTASSERTION_H
