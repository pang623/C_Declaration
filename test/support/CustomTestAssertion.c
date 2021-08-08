#include "CustomTestAssertion.h"

char *createMessage(char *message, ...) {
	size_t len;
	char *buffer;

	va_list va;
	va_start(va, message);
	len = vsnprintf(NULL, 0, message, va);
	buffer = malloc(len + 1);
	vsprintf(buffer, message, va);
	va_end(va);

	return buffer;
}

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
  UNITY_TEST_ASSERT((symbolId == root->id), lineNum, 
                     createMessage("Expecting symbol ID %d, but is %d instead", symbolId, root->id));
  UNITY_TEST_ASSERT((INFIX == root->arity), lineNum, 
                     createMessage("Expecting arity of INFIX, but is %d instead", root->arity));
  UNITY_TEST_ASSERT(!(stricmp(symbol, root->token->str)), lineNum, 
                     createMessage("Expecting symbol %s at root, but is %s instead", symbol, root->token->str));
  
  TestSymbolType *testChild = childZero;
  for(int i = 0; i < 2; i++) {
    if(testChild->type == OPERATOR_TYPE)
      UNITY_TEST_ASSERT(!(stricmp(((TestOperator *)testChild)->_operator, root->child[i]->token->str)), lineNum, 
                        createMessage("Expecting operator %s at child %d, but is %s instead", 
                        ((TestOperator *)testChild)->_operator, i, root->child[i]->token->str));
    else if(testChild->type == NUMBER_TYPE)
      UNITY_TEST_ASSERT(!(stricmp(((TestNumber *)testChild)->value, root->child[i]->token->str)), lineNum, 
                        createMessage("Expecting number %s at child %d, but is %s instead",
                        ((TestNumber *)testChild)->value, i, root->child[i]->token->str));
    else
      UNITY_TEST_ASSERT(!(stricmp(((TestIdentifier *)testChild)->str, root->child[i]->token->str)), lineNum, 
                        createMessage("Expecting identifier %s at child %d, but is %s instead", 
                        ((TestIdentifier *)testChild)->str, i, root->child[i]->token->str));
    testChild = childOne;
  }
  free(childZero);
  free(childOne);
}

void testAssertSingleOperand(int symbolId, char *symbol, int arity, TestSymbolType *childZero, Symbol *root, int lineNum) {
  UNITY_TEST_ASSERT((symbolId == root->id), lineNum, 
                     createMessage("Expecting symbol ID %d, but is %d instead", symbolId, root->id));
  UNITY_TEST_ASSERT((arity == root->arity), lineNum, 
                     createMessage("Expecting arity of %d, but is %d instead", arity, root->arity));
  UNITY_TEST_ASSERT(!(stricmp(symbol, root->token->str)), lineNum, 
                     createMessage("Expecting symbol %s at root, but is %s instead", symbol, root->token->str));

  TestSymbolType *testChild = childZero;
  if(testChild->type == OPERATOR_TYPE)
    UNITY_TEST_ASSERT(!(stricmp(((TestOperator *)testChild)->_operator, root->child[0]->token->str)), lineNum, 
                        createMessage("Expecting operator %s at child 0, but is %s instead",
                        ((TestOperator *)testChild)->_operator, root->child[0]->token->str));
  else if(testChild->type == NUMBER_TYPE)
    UNITY_TEST_ASSERT(!(stricmp(((TestNumber *)testChild)->value, root->child[0]->token->str)), lineNum, 
                        createMessage("Expecting number %s at child 0, but is %s instead", 
                        ((TestNumber *)testChild)->value, root->child[0]->token->str));
  else
    UNITY_TEST_ASSERT(!(stricmp(((TestIdentifier *)testChild)->str, root->child[0]->token->str)), lineNum, 
                        createMessage("Expecting identifier %s at child 0, but is %s instead",
                        ((TestIdentifier *)testChild)->str, root->child[0]->token->str));
  free(childZero);
}