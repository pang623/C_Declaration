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
  Symbol *symbol;
  tokenizer = createTokenizer("3 +2 *   4");
  Try {
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
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_NEG3_MINUS_2_times_NEG4_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("-3 -+2 *-   4");
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("-", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("-", symbol->left->token->str);
    TEST_ASSERT_EQUAL(PREFIX, symbol->left->arity);
    TEST_ASSERT_NULL(symbol->left->left);
    TEST_ASSERT_EQUAL_STRING("3", symbol->left->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->left->right->arity);
    TEST_ASSERT_EQUAL_STRING("*", symbol->right->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->right->arity);
    TEST_ASSERT_EQUAL_STRING("+", symbol->right->left->token->str);
    TEST_ASSERT_EQUAL(PREFIX, symbol->right->left->arity);
    TEST_ASSERT_NULL(symbol->right->left->left);
    TEST_ASSERT_EQUAL_STRING("2", symbol->right->left->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->right->left->right->arity);
    TEST_ASSERT_EQUAL_STRING("-", symbol->right->right->token->str);
    TEST_ASSERT_EQUAL(PREFIX, symbol->right->right->arity);
    TEST_ASSERT_NULL(symbol->right->right->left);
    TEST_ASSERT_EQUAL_STRING("4", symbol->right->right->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->right->right->right->arity);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

#endif // TEST
