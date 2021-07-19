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
    /*
    //Test tree created is correct in order
    TEST_ASSERT_SUB_AST(Integer(3), Operator("+"), Operator("*"), astRoot);
    TEST_ASSERT_SUB_AST(NULL, Integer(3), NULL, astRoot->left);
    TEST_ASSERT_SUB_AST(Integer(2), Operator("*"), Integer(4), astRoot->right);
    TEST_ASSERT_SUB_AST(NULL, Integer(2), NULL, astRoot->right->left);
    TEST_ASSERT_SUB_AST(NULL, Integer(4), NULL, astRoot->right->right);
    */
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

void test_expression_given_3_postincrement_plus_2_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3+++2");
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("++", symbol->left->token->str);
    TEST_ASSERT_EQUAL(SUFFIX, symbol->left->arity);
    TEST_ASSERT_EQUAL_STRING("3", symbol->left->left->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->left->left->arity);
    TEST_ASSERT_NULL(symbol->left->right);
    TEST_ASSERT_EQUAL_STRING("2", symbol->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->right->arity);
    TEST_ASSERT_NULL(symbol->right->left);
    TEST_ASSERT_NULL(symbol->right->right);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

/*
//this is evaluated to (3++)++ + 2, which is invalid
void test_expression_given_3_doublepostincrement_plus_2_expect_ERR_SYNTAX_thrown(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3+++++2");
  Try{
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

//(--3)++ is also invalid
void test_expression_given_predecrement_3_plus_2_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("--3+++2");
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("++", symbol->left->token->str);
    TEST_ASSERT_EQUAL(SUFFIX, symbol->left->arity);
    TEST_ASSERT_EQUAL_STRING("3", symbol->left->left->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->left->left->arity);
    TEST_ASSERT_NULL(symbol->left->right);
    TEST_ASSERT_EQUAL_STRING("2", symbol->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->right->arity);
    TEST_ASSERT_NULL(symbol->right->left);
    TEST_ASSERT_NULL(symbol->right->right);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_3_and_2_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3 2");
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("++", symbol->left->token->str);
    TEST_ASSERT_EQUAL(SUFFIX, symbol->left->arity);
    TEST_ASSERT_EQUAL_STRING("3", symbol->left->left->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->left->left->arity);
    TEST_ASSERT_NULL(symbol->left->right);
    TEST_ASSERT_EQUAL_STRING("2", symbol->right->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->right->arity);
    TEST_ASSERT_NULL(symbol->right->left);
    TEST_ASSERT_NULL(symbol->right->right);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}
*/
#endif // TEST
