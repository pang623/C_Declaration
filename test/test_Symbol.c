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

/*----------CASE 1 -> Symbol with double repeated char, and with equal sign as last char ----------*/

void test_underscore_getSymbol_given_a_double_repeated_but_separated_char_symbol_expect_symbol_returned_is_not_the_whole_operator() {
  tokenizer = createTokenizer(" + +   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(ADD, symbol->id);
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_inc_symbol_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" ++ ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  //Will be changed to INC_AFTER once it is determined that it is a prefix inside TDOP module
  TEST_ASSERT_EQUAL(INC_BEFORE, symbol->id);
  TEST_ASSERT_EQUAL_STRING("++", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_as_last_char_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" -=* ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(SUBT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("-=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_that_has_equal_as_last_char_but_adjacent_with_another_equal_expect_symbol_returned_is_without_the_extra_equal() {
  tokenizer = createTokenizer(" *==   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(MUL_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("*=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_as_last_char_but_invalid_expect_symbol_returned_is_without_the_equal_sign() {
  tokenizer = createTokenizer(" ++= ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(INC_BEFORE, symbol->id);
  TEST_ASSERT_EQUAL_STRING("++", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

/*----------CASE 2 -> Symbol with only equal sign as last char ----------*/

void test_underscore_getSymbol_given_a_double_repeated_char_symbol_but_separated_expect_symbol_returned_is_assignment() {
  tokenizer = createTokenizer(" = =");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(ASSIGNMENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("=", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_but_separated_expect_symbol_returned_is_only_the_symbol_without_equal() {
  tokenizer = createTokenizer(" ! = ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(LOGI_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("!", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_an_adjacent_double_char_symbol_but_is_invalid_expect_symbol_returned_is_only_the_single_char_symbol() {
  tokenizer = createTokenizer(" !! ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(LOGI_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("!", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_expect_symbol_returned_is_correct() {
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

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_but_different_symbols_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" /= ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(DIV_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("/=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_symbol_that_only_valid_with_equal_sign_as_last_char_but_adjacent_with_extra_equal_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" /== ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(DIV_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("/=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

/*----------CASE 3 -> Symbol that only exist by itself (do not combine with other symbols) ----------*/

void test_underscore_getSymbol_given_symbol_bit_NOT_but_is_adjacent_with_another_tilda_expect_symbol_returned_is_still_only_a_single_tilda() {
  tokenizer = createTokenizer(" ~~~ ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(BIT_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("~", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_symbol_bit_NOT_but_is_adjacent_with_equal_from_expect_symbol_returned_is_still_only_a_single_tilda() {
  tokenizer = createTokenizer(" ~= ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(BIT_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("~", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_adjacent_open_parent_expect_symbol_returned_is_only_single_open_parent() {
  tokenizer = createTokenizer(" ((   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(OPEN_PARENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("(", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_open_parent_but_with_adjacent_equal_expect_symbol_returned_is_only_single_open_parent() {
  tokenizer = createTokenizer(" (=   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(OPEN_PARENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("(", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

/*----------CASE 4 -> Symbol that overlap with case 1, case 2, case 3 plus equal sign appended to double repeated char----------*/

void test_underscore_getSymbol_given_a_separated_relational_operator_expect_symbol_returned_is_without_the_equal_sign() {
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

void test_underscore_getSymbol_given_a_relational_operator_but_adjacent_with_equal_expect_symbol_returned_is_just_the_relational_operator() {
  tokenizer = createTokenizer(" >==   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(GREATER_OR_EQUAL, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_triple_char_symbol_expect_symbol_returned_is_correct() {
  tokenizer = createTokenizer(" >>=");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(R_SHIFT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">>=", symbol->token->str);
  TEST_ASSERT_EQUAL(3, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_separated_three_char_assignment_operator_double_char_separated_expect_symbol_returned_is_not_the_whole_assignment_operator() {
  tokenizer = createTokenizer(" < <=   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(LESSER, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_a_separated_three_char_assignment_operator_equal_separated_expect_symbol_returned_is_not_the_whole_assignment_operator() {
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

void test_underscore_getSymbol_given_a_triple_char_symbol_but_adjacent_with_another_equal_expect_symbol_returned_is_just_the_triple_char_operator() {
  tokenizer = createTokenizer(" <<==   ");
  Symbol *symbol;
  
  symbol = _getSymbol(tokenizer);
  TEST_ASSERT_EQUAL(L_SHIFT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<<=", symbol->token->str);
  TEST_ASSERT_EQUAL(3, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

#endif // TEST
