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

void testAssertInfix(int symbolId, char *symbol, TestSymbolType *childZero, TestSymbolType *childOne, Symbol *root, int lineNum) {
  UNITY_TEST_ASSERT_EQUAL_INT(symbolId, root->id, lineNum, NULL);
  UNITY_TEST_ASSERT_EQUAL_INT(INFIX, root->arity, lineNum, NULL);
  UNITY_TEST_ASSERT_EQUAL_STRING(symbol, root->token->str, lineNum, NULL);
  
  TestSymbolType *testChild = childZero;
  for(int i = 0; i < 2; i++) {
    if(testChild->type == OPERATOR_TYPE)
      UNITY_TEST_ASSERT_EQUAL_STRING(((TestOperator *)testChild)->_operator, root->child[i]->token->str, lineNum, NULL);
    else if(testChild->type == NUMBER_TYPE)
      UNITY_TEST_ASSERT_EQUAL_STRING(((TestNumber *)testChild)->value, root->child[i]->token->str, lineNum, NULL);
    else
      UNITY_TEST_ASSERT_EQUAL_STRING(((TestIdentifier *)testChild)->str, root->child[i]->token->str, lineNum, NULL);
    testChild = childOne;
  }
  free(childZero);
  free(childOne);
}

void testAssertSingleOperand(int symbolId, char *symbol, int arity, TestSymbolType *childZero, Symbol *root, int lineNum) {
  UNITY_TEST_ASSERT_EQUAL_INT(symbolId, root->id, lineNum, NULL);
  UNITY_TEST_ASSERT_EQUAL_INT(arity, root->arity, lineNum, NULL);
  UNITY_TEST_ASSERT_EQUAL_STRING(symbol, root->token->str, lineNum, NULL);
  
  TestSymbolType *testChild = childZero;
  if(testChild->type == OPERATOR_TYPE)
    UNITY_TEST_ASSERT_EQUAL_STRING(((TestOperator *)testChild)->_operator, root->child[0]->token->str, lineNum, NULL);
  else if(testChild->type == NUMBER_TYPE)
    UNITY_TEST_ASSERT_EQUAL_STRING(((TestNumber *)testChild)->value, root->child[0]->token->str, lineNum, NULL);
  else
    UNITY_TEST_ASSERT_EQUAL_STRING(((TestIdentifier *)testChild)->str, root->child[0]->token->str, lineNum, NULL);
  free(childZero);
}