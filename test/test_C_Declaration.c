#ifdef TEST

#include "unity.h"
#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Errors.h"
#include "TokenizerExceptionThrowing.h"
#include "C_Declaration.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern Tokenizer *tokenizer;

void test_expression_given_3_plus_2_times_4_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3 +2 *   4");
  symbol = expression(0);
  //Test tree created is correct in order
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL_STRING("3", symbol->left->token->str);
  TEST_ASSERT_NULL(symbol->left->left);
  TEST_ASSERT_NULL(symbol->left->right);
  TEST_ASSERT_EQUAL_STRING("*", symbol->right->token->str);
  TEST_ASSERT_EQUAL_STRING("2", symbol->right->left->token->str);
  TEST_ASSERT_NULL(symbol->right->left->left);
  TEST_ASSERT_NULL(symbol->right->left->right);
  TEST_ASSERT_EQUAL_STRING("4", symbol->right->right->token->str);
  TEST_ASSERT_NULL(symbol->right->right->left);
  TEST_ASSERT_NULL(symbol->right->right->right);
  freeTokenizer(tokenizer);
}

#endif // TEST
