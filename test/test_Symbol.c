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

void test_createSymbol_given_symbol_data_expect_symbol_created() {
  Token *token = NULL;
  Symbol *symbol;
  tokenizer = createTokenizer("*");
  token = getToken(tokenizer);
  Symbol symbolData = {INFIX, MULTIPLY, token};
  
  symbol = createSymbol(&symbolData);
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  TEST_ASSERT_EQUAL_STRING("*", symbol->token->str);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);
  TEST_ASSERT_EQUAL(MULTIPLY, symbol->id);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_triple_char_symbol_from_tokenizer_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" >>=");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(SH_R_SHIFT, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">>=", symbol->token->str);
  TEST_ASSERT_EQUAL(3, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_double_repeated_char_symbol_from_tokenizer_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" == ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(EQUALITY, symbol->id);
  TEST_ASSERT_EQUAL_STRING("==", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_double_char_symbol_from_tokenizer_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" /=   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(SH_DIV, symbol->id);
  TEST_ASSERT_EQUAL_STRING("/=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_separated_relational_operator_from_tokenizer_expect_symbol_returned_is_not_relational_operator() {
  tokenizer = createTokenizer(" > =   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(GREATER, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_separated_three_char_assignment_operator_from_tokenizer_expect_symbol_returned_is_not_the_whole_assignment_operator() {
  tokenizer = createTokenizer(" << =   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(L_SHIFT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<<", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

#endif // TEST
