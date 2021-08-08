#include "CustomTestAssertion.h"

TestSymbolType *Number(char *value) {
  TestNumber *symbol = (TestNumber *)malloc(sizeof(TestNumber));
  symbol->type = NUMBER_TYPE;
  symbol->value = value;
  return (TestSymbolType *)symbol;
}

TestSymbolType *Identifier(char *str) {
  TestIdentifier *symbol = (TestIdentifier *)malloc(sizeof(TestIdentifier));
  symbol->type = IDENTIFIER_TYPE;
  symbol->str = str;
  return (TestSymbolType *)symbol;
}

TestSymbolType *Operator(char *_operator) {
  TestOperator *symbol = (TestOperator *)malloc(sizeof(TestOperator));
  symbol->type = OPERATOR_TYPE;
  symbol->_operator = _operator;
  return (TestSymbolType *)symbol;
}

void testAssertInfix(int symbolId, char *symbol, TestSymbolType *childZero, TestSymbolType *childOne, Symbol *root) {
  TEST_ASSERT_EQUAL(symbolId, root->id);
  TEST_ASSERT_EQUAL(INFIX, root->arity);
  TEST_ASSERT_EQUAL_STRING(symbol, root->token->str);
  
  TestSymbolType *testChild = childZero;
  for(int i = 0; i < 2; i++) {
    if(testChild->type == OPERATOR_TYPE)
      TEST_ASSERT_EQUAL_STRING(((TestOperator *)testChild)->_operator, root->child[i]->token->str);
    else if(testChild->type == NUMBER_TYPE)
      TEST_ASSERT_EQUAL_STRING(((TestNumber *)testChild)->value, root->child[i]->token->str);
    else
      TEST_ASSERT_EQUAL_STRING(((TestIdentifier *)testChild)->str, root->child[i]->token->str);
    testChild = childOne;
  }
  free(childZero);
  free(childOne);
}

void testAssertSingleOperand(int symbolId, char *symbol, int arity, TestSymbolType *childZero, Symbol *root) {
  TEST_ASSERT_EQUAL(symbolId, root->id);
  TEST_ASSERT_EQUAL(arity, root->arity);
  TEST_ASSERT_EQUAL_STRING(symbol, root->token->str);
  
  TestSymbolType *testChild = childZero;
  if(testChild->type == OPERATOR_TYPE)
    TEST_ASSERT_EQUAL_STRING(((TestOperator *)testChild)->_operator, root->child[0]->token->str);
  else if(testChild->type == NUMBER_TYPE)
    TEST_ASSERT_EQUAL_STRING(((TestNumber *)testChild)->value, root->child[0]->token->str);
  else
    TEST_ASSERT_EQUAL_STRING(((TestIdentifier *)testChild)->str, root->child[0]->token->str);
  
  free(childZero);
}