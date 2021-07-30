#ifdef TEST

#include "unity.h"
#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Errors.h"
#include "TokenizerExceptionThrowing.h"
#include "C_Declaration.h"
#include "CDecl_Errors.h"
#include "Symbol.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "Exception.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;
/*
void test_createSymbol_given_symbol_data_expect_symbol_created() {
  Token *token = NULL;
  Symbol *symbol;
  tokenizer = createTokenizer("*");
  token = getToken(tokenizer);
  Symbol symbolData = {NULL, NULL, token, INFIX, MULTIPLY};
  
  symbol = createSymbol(&symbolData);
  TEST_ASSERT_NULL(symbol->left);
  TEST_ASSERT_NULL(symbol->right);
  TEST_ASSERT_EQUAL_STRING("*", symbol->token->str);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);
  TEST_ASSERT_EQUAL(MULTIPLY, symbol->id);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}
*/
#endif // TEST
