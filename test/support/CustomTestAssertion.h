#ifndef CUSTOMTESTASSERTION_H
#define CUSTOMTESTASSERTION_H

#include "unity.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Arity.h"
#include "Symbol.h"
#include "SymbolTable.h"

//TestSymbol Types
#define   OPERATOR_TYPE       10
#define   NUMBER_TYPE         11
#define   IDENTIFIER_TYPE     12

//TEST_ASSERT MACROS
#define   TEST_ASSERT_SYMBOL(expectedSymbolId, expectedSymbolChar, childZero, childOne, root)      \
            testAssertSymbol(expectedSymbolId, expectedSymbolChar, childZero, childOne, root, __LINE__)
            
#define   isOperatorNotMatch(testChild, child)   \
            ((child == NULL) || (stricmp(((TestOperator *)testChild)->_operator, child->token->str)))
            
#define   isNumberNotMatch(testChild, child)   \
            ((child == NULL) || (stricmp(((TestNumber *)testChild)->value, child->token->str)))

#define   isIdentifierNotMatch(testChild, child)   \
            ((child == NULL) || (stricmp(((TestIdentifier *)testChild)->str, child->token->str)))

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
