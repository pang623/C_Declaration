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

void testAssertSymbol(int symbolId, char *symbol, TestSymbolType *childZero, TestSymbolType *childOne, Symbol *root, int lineNum) {
  UNITY_TEST_ASSERT((symbolId == root->id), lineNum, 
                     createMessage("Expecting symbol ID %d, but is %d instead", symbolId, root->id));
  UNITY_TEST_ASSERT(!(stricmp(symbol, root->token->str)), lineNum, 
                     createMessage("Expecting symbol %s at root, but is %s instead", symbol, root->token->str));
  
  char *actualStr;
  TestSymbolType *testChild = childZero;
  for(int i = 0; i < 2; i++) {
    if(root->child[i])
      actualStr = root->child[i]->token->str;
    else
      actualStr = NULL;
    
    if(!testChild)
      UNITY_TEST_ASSERT_NULL(root->child[i], lineNum, 
                        createMessage("NOT expecting a symbol %s here", root->child[i]->token->str));
    else if(testChild->type == OPERATOR_TYPE)
      UNITY_TEST_ASSERT(!isOperatorNotMatch(testChild, root->child[i]), lineNum, 
                        createMessage("Expecting operator %s at child %d, but is %s instead", 
                        ((TestOperator *)testChild)->_operator, i, actualStr));
    else if(testChild->type == NUMBER_TYPE)
      UNITY_TEST_ASSERT(!isNumberNotMatch(testChild, root->child[i]), lineNum, 
                        createMessage("Expecting number %s at child %d, but is %s instead",
                        ((TestNumber *)testChild)->value, i, actualStr));
    else
      UNITY_TEST_ASSERT(!isIdentifierNotMatch(testChild, root->child[i]), lineNum, 
                        createMessage("Expecting identifier %s at child %d, but is %s instead", 
                        ((TestIdentifier *)testChild)->str, i, actualStr));
    testChild = childOne;
  }
  if(childZero)
    free(childZero);
  if(childOne)
    free(childOne);
}