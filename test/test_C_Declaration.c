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
  TEST_IGNORE();
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

void test_getBP_given_token_and_find_lbp_expect_its_lbp_returned() {
  Token *token = NULL;
  tokenizer = createTokenizer("+");
  token = getToken(tokenizer);
  Try {
    int bp = 0;
    bp = getBP(LEFTBP, token);
    TEST_ASSERT_EQUAL(9, bp);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_getBP_given_token_and_find_rbp_expect_its_rbp_returned() {
  Token *token = NULL;
  tokenizer = createTokenizer("/");
  token = getToken(tokenizer);
  Try {
    int bp = 0;
    bp = getBP(RIGHTBP, token);
    TEST_ASSERT_EQUAL(20, bp);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_getBP_given_invalid_symbol_as_token_expect_exception_thrown() {
  Token *token = NULL;
  tokenizer = createTokenizer("$");
  token = getToken(tokenizer);
  Try {
    int bp = 0;
    bp = getBP(RIGHTBP, token);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

#endif // TEST
