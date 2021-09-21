#ifdef TEST

#include "unity.h"
#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Errors.h"
#include "TokenizerExceptionThrowing.h"
#include "SymbolAttrTable.h"
#include "CDecl_Errors.h"
#include "Symbol.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "Exception.h"
#include "Strings.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;
SymbolParser *symbolParser;

void test_createSymbol_given_symbol_data_expect_symbol_created() {
  Token *token = NULL;
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("*");
  token = getToken(tokenizer);
  Symbol symbolData = {INFIX, MULTIPLY, token};
  
  symbol = createSymbol(&symbolData);
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  TEST_ASSERT_EQUAL_STRING("*", symbol->token->str);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);
  TEST_ASSERT_EQUAL(MULTIPLY, symbol->id);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*----------CASE 1 -> Symbol with double repeated char, and with equal sign as last char ----------*/

void test_underscore_getSymbol_given_a_double_repeated_but_separated_char_symbol_expect_symbol_returned_is_not_the_whole_operator() {
  Tokenizer *tokenizer = createTokenizer(" + +   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(ADD, symbol->id);
  TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_inc_symbol_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" ++ ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  //Will be changed to INC_AFTER once it is determined that it is a prefix inside TDOP module
  TEST_ASSERT_EQUAL(INC_BEFORE, symbol->id);
  TEST_ASSERT_EQUAL_STRING("++", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_as_last_char_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" -=* ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(SUBT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("-=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_that_has_equal_as_last_char_but_adjacent_with_another_equal_expect_symbol_returned_is_without_the_extra_equal() {
  Tokenizer *tokenizer = createTokenizer(" *==   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(MUL_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("*=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_as_last_char_but_invalid_expect_symbol_returned_is_without_the_equal_sign() {
  Tokenizer *tokenizer = createTokenizer(" ++= ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(INC_BEFORE, symbol->id);
  TEST_ASSERT_EQUAL_STRING("++", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*----------CASE 2 -> Symbol with only equal sign as last char ----------*/

void test_underscore_getSymbol_given_a_double_repeated_char_symbol_but_separated_expect_symbol_returned_is_assignment() {
  Tokenizer *tokenizer = createTokenizer(" = =");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(ASSIGNMENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("=", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_but_separated_expect_symbol_returned_is_only_the_symbol_without_equal() {
  Tokenizer *tokenizer = createTokenizer(" ! = ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(LOGI_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("!", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_an_adjacent_double_char_symbol_but_is_invalid_expect_symbol_returned_is_only_the_single_char_symbol() {
  Tokenizer *tokenizer = createTokenizer(" !! ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(LOGI_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("!", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" == ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(EQUALITY, symbol->id);
  TEST_ASSERT_EQUAL_STRING("==", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_with_equal_sign_as_last_char_but_different_symbols_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" /= ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(DIV_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("/=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_symbol_that_only_valid_with_equal_sign_as_last_char_but_adjacent_with_extra_equal_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" /== ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(DIV_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("/=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*----------CASE 3 -> Symbol that only exist by itself (do not combine with other symbols) ----------*/

void test_underscore_getSymbol_given_symbol_bit_NOT_but_is_adjacent_with_another_tilda_expect_symbol_returned_is_still_only_a_single_tilda() {
  Tokenizer *tokenizer = createTokenizer(" ~~~ ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(BIT_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("~", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_symbol_bit_NOT_but_is_adjacent_with_equal_from_expect_symbol_returned_is_still_only_a_single_tilda() {
  Tokenizer *tokenizer = createTokenizer(" ~= ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(BIT_NOT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("~", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_adjacent_open_parent_expect_symbol_returned_is_only_single_open_parent() {
  Tokenizer *tokenizer = createTokenizer(" ((   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(OPEN_PARENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("(", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_open_parent_but_with_adjacent_equal_expect_symbol_returned_is_only_single_open_parent() {
  Tokenizer *tokenizer = createTokenizer(" (=   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(OPEN_PARENT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("(", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*----------CASE 4 -> Symbol that overlap with case 1, case 2, case 3 plus equal sign appended to double repeated char----------*/

void test_underscore_getSymbol_given_a_separated_relational_operator_expect_symbol_returned_is_without_the_equal_sign() {
  Tokenizer *tokenizer = createTokenizer(" > =   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(GREATER, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_relational_operator_but_adjacent_with_equal_expect_symbol_returned_is_just_the_relational_operator() {
  Tokenizer *tokenizer = createTokenizer(" >==   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(GREATER_OR_EQUAL, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">=", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_triple_char_symbol_expect_symbol_returned_is_correct() {
  Tokenizer *tokenizer = createTokenizer(" >>=");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(R_SHIFT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING(">>=", symbol->token->str);
  TEST_ASSERT_EQUAL(3, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_separated_three_char_assignment_operator_double_char_separated_expect_symbol_returned_is_not_the_whole_assignment_operator() {
  Tokenizer *tokenizer = createTokenizer(" < <=   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(LESSER, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<", symbol->token->str);
  TEST_ASSERT_EQUAL(1, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_separated_three_char_assignment_operator_equal_separated_expect_symbol_returned_is_not_the_whole_assignment_operator() {
  Tokenizer *tokenizer = createTokenizer(" << =   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(L_SHIFT, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<<", symbol->token->str);
  TEST_ASSERT_EQUAL(2, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_underscore_getSymbol_given_a_triple_char_symbol_but_adjacent_with_another_equal_expect_symbol_returned_is_just_the_triple_char_operator() {
  Tokenizer *tokenizer = createTokenizer(" <<==   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  symbol = _getSymbol(symbolParser->tokenizer);
  TEST_ASSERT_EQUAL(L_SHIFT_ASSIGN, symbol->id);
  TEST_ASSERT_EQUAL_STRING("<<=", symbol->token->str);
  TEST_ASSERT_EQUAL(3, symbol->token->length);
  TEST_ASSERT_EQUAL(INFIX, symbol->arity);  //INFIX arity by default
  TEST_ASSERT_NULL(symbol->child[0]);
  TEST_ASSERT_NULL(symbol->child[1]);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_peek_symbol_given_an_empty_stack_expect_symbol_peeked_is_stored_in_stack() {
  Tokenizer *tokenizer = createTokenizer(" |=   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  //Verify is empty stack
  TEST_ASSERT_EQUAL(0, symbolParser->symbolStack->count);
  TEST_ASSERT_NULL(symbolParser->symbolStack->head);
  
  //Peek the symbol, symbol peeked should be pushed to the stack
  symbol = peekSymbol(symbolParser);
  
  //Check if the symbol is pushed to the stack
  TEST_ASSERT_EQUAL(1, symbolParser->symbolStack->count);
  TEST_ASSERT_EQUAL(symbol, symbolParser->symbolStack->head->data);
  
  freeSymbolParser(symbolParser);
}

void test_peek_symbol_given_a_stack_with_symbol_expect_symbol_is_still_in_stack_after_peeked() {
  Tokenizer *tokenizer = createTokenizer(" |=   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  //Verify is empty stack
  TEST_ASSERT_EQUAL(0, symbolParser->symbolStack->count);
  TEST_ASSERT_NULL(symbolParser->symbolStack->head);
  
  //Peek the symbol, symbol peeked should be pushed to the stack
  symbol = peekSymbol(symbolParser);
  
  //Check if the symbol is pushed to the stack
  TEST_ASSERT_EQUAL(1, symbolParser->symbolStack->count);
  TEST_ASSERT_EQUAL(symbol, symbolParser->symbolStack->head->data);
  
  //Peek the symbol again, this time with symbol in stack
  symbol = peekSymbol(symbolParser);
  
  //Verify that peek symbol does not consume the symbol
  //The stack should still contain the symbol
  TEST_ASSERT_EQUAL(1, symbolParser->symbolStack->count);
  TEST_ASSERT_EQUAL(symbol, symbolParser->symbolStack->head->data);
  
  freeSymbolParser(symbolParser);
}

void test_get_symbol_given_a_stack_with_symbol_expect_symbol_is_not_in_stack_after_getSymbol() {
  Tokenizer *tokenizer = createTokenizer(" |=   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbolPeek, *symbolGet;
  
  //Verify is empty stack
  TEST_ASSERT_EQUAL(0, symbolParser->symbolStack->count);
  TEST_ASSERT_NULL(symbolParser->symbolStack->head);
  
  //Peek the symbol, symbol peeked should be pushed to the stack
  symbolPeek = peekSymbol(symbolParser);
  
  //Check if the symbol is pushed to the stack
  TEST_ASSERT_EQUAL(1, symbolParser->symbolStack->count);
  TEST_ASSERT_EQUAL(symbolPeek, symbolParser->symbolStack->head->data);
  
  //This time get the symbol, this should pop the symbol off the stack
  symbolGet = getSymbol(symbolParser);
  
  //Verify that stack is empty after getSymbol
  //The symbol returned should be the one in the stack
  TEST_ASSERT_EQUAL(0, symbolParser->symbolStack->count);
  TEST_ASSERT_EQUAL_PTR(symbolPeek, symbolGet);
  
  freeSymbol(symbolGet);
  freeSymbolParser(symbolParser);
}

void test_isNextSymbolThenConsume_given_next_symbol_is_desired_expect_it_to_be_consumed() {
  Tokenizer *tokenizer = createTokenizer(" !ab  ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  //Result should be true as the upcoming symbol is "!"
  int result = isNextSymbolThenConsume(LOGI_NOT);
  TEST_ASSERT_EQUAL(1, result);
  
  //Get the next symbol, should be "ab" as "!" was consumed
  symbol = getSymbol(symbolParser);
  
  //Check if next symbol is "ab" after "!" was consumed
  TEST_ASSERT_EQUAL(IDENTIFIER, symbol->id);
  TEST_ASSERT_EQUAL_STRING("ab", symbol->token->str);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isNextSymbolThenConsume_given_next_symbol_is_not_desired_expect_it_is_not_consumed() {
  Tokenizer *tokenizer = createTokenizer(" 3-a  ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  //Result should be false as the upcoming symbol is "3"
  int result = isNextSymbolThenConsume(SUBTRACT);
  TEST_ASSERT_EQUAL(0, result);
  
  //Get the next symbol, should still be "3" as it is not consumed
  symbol = getSymbol(symbolParser);
  
  //Check if next symbol is "3"
  TEST_ASSERT_EQUAL(NUMBER, symbol->id);
  TEST_ASSERT_EQUAL_STRING("3", symbol->token->str);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_verifyIsNextSymbolThenConsume_given_next_symbol_is_desired_expect_it_to_be_consumed() {
  Tokenizer *tokenizer = createTokenizer(" >>=3  ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol;
  
  verifyIsNextSymbolThenConsume(R_SHIFT_ASSIGN, ">>=");
  
  //Get the next symbol, should be "3" as ">>=" was consumed
  symbol = getSymbol(symbolParser);
  
  //Check if next symbol is "3" after ">>=" was consumed
  TEST_ASSERT_EQUAL(NUMBER, symbol->id);
  TEST_ASSERT_EQUAL_STRING("3", symbol->token->str);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeyword_given_symbol_not_a_keyword_and_identifier_is_seen_as_keyword_is_disabled_expect_result_returned_is_false() {
  Tokenizer *tokenizer = createTokenizer(" apple ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  int result = isSymbolKeyword(symbol, 0);
  TEST_ASSERT_EQUAL(0, result);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeyword_given_symbol_not_a_keyword_but_identifier_is_seen_as_keyword_is_enabled_expect_result_returned_is_true() {
  Tokenizer *tokenizer = createTokenizer(" Tokenizer ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  //Although Symbol is not a standard C data type keyword, here it is assumed to be a keyword
  //Identifiers that are not standard C keywords will only be seen as keyword if the identifierIsSeenAsKeyword is enabled
  int result = isSymbolKeyword(symbol, 1);
  TEST_ASSERT_EQUAL(1, result);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeyword_given_symbol_is_a_keyword_expect_result_returned_is_true() {
  Tokenizer *tokenizer = createTokenizer(" double ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  //Here identifierIsSeenAsKeyword is disabled, but then "double" is a standard C keyword
  //So result will return true
  int result = isSymbolKeyword(symbol, 0);
  TEST_ASSERT_EQUAL(1, result);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeywordThenGetType_given_symbol_is_a_keyword_expect_keyword_type_is_obtained_and_result_returns_true() {
  Tokenizer *tokenizer = createTokenizer(" while ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  int i = 0;
  int *type = &i;
  int result = isSymbolKeywordThenGetType(symbol, type, 0);
  TEST_ASSERT_EQUAL(1, result);
  //The keyword is a while loop keyword, so WHILE should be returned
  TEST_ASSERT_EQUAL(WHILE, *type);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeywordThenGetType_given_symbol_is_not_keyword_expect_keyword_type_is_not_obtained_and_result_returns_false() {
  Tokenizer *tokenizer = createTokenizer(" bambi ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  int i = 3;
  int *type = &i;
  int result = isSymbolKeywordThenGetType(symbol, type, 0);
  TEST_ASSERT_EQUAL(0, result);
  //The keyword is not a keyword, so keyword type is not returned
  TEST_ASSERT_EQUAL(3, *type);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isSymbolKeywordThenGetType_given_symbol_is_not_keyword_but_identifierIsSeenAsKeyword_enabled_expect_keyword_type_is_obtained_and_result_returns_true() {
  Tokenizer *tokenizer = createTokenizer(" Simba ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = getSymbol(symbolParser);
  
  int i = 0;
  int *type = &i;
  int result = isSymbolKeywordThenGetType(symbol, type, 1);
  TEST_ASSERT_EQUAL(1, result);
  //Non-keyword identifiers seen as keyword, type is returned
  TEST_ASSERT_EQUAL(TYPE, *type);
  
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_isCorrectSymbolAndAdjacent_given_symbol_adjacent_repeating_expect_result_returns_true() {
  Tokenizer *tokenizer = createTokenizer(" >>+ ");
  Token *symbol = getToken(tokenizer);
  Token *nextSymbol = getToken(tokenizer);
  
  //if symToCheck is NULL, it means that the function will check the next token for repeating symbol based on the first token
  int result = isCorrectSymbolAndAdjacent(symbol, nextSymbol, NULL);
  TEST_ASSERT_EQUAL(1, result);
  
  freeToken(symbol);
  freeToken(nextSymbol);
  freeTokenizer(tokenizer);
}

void test_isCorrectSymbolAndAdjacent_given_symbol_adjacent_equal_sign_expect_result_returns_true() {
  Tokenizer *tokenizer = createTokenizer(" >= ");
  Token *symbol = getToken(tokenizer);
  Token *nextSymbol = getToken(tokenizer);
  
  //next symbol to check is "="
  int result = isCorrectSymbolAndAdjacent(symbol, nextSymbol, "=");
  TEST_ASSERT_EQUAL(1, result);
  
  freeToken(symbol);
  freeToken(nextSymbol);
  freeTokenizer(tokenizer);
}

void test_isCorrectSymbolAndAdjacent_given_symbol_adjacent_is_not_equal_sign_expect_result_returns_false() {
  Tokenizer *tokenizer = createTokenizer(" >> ");
  Token *symbol = getToken(tokenizer);
  Token *nextSymbol = getToken(tokenizer);
  
  //next symbol to check is "=", but is not "=", return false
  int result = isCorrectSymbolAndAdjacent(symbol, nextSymbol, "=");
  TEST_ASSERT_EQUAL(0, result);
  
  freeToken(symbol);
  freeToken(nextSymbol);
  freeTokenizer(tokenizer);
}

void test_underscore_getSymbol_given_unsupported_symbol_expect_ERR_INVALID_SYMBOL_is_thrown() {
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer(" $ ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = _getSymbol(symbolParser->tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_verifyIsNextSymbolThenConsume_given_next_symbol_is_not_desired_expect_ERR_WRONG_SYMBOL_is_thrown() {
  Tokenizer *tokenizer = createTokenizer(" 3.5+a ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    verifyIsNextSymbolThenConsume(ADD, "+");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_WRONG_SYMBOL, e->errorCode);
    free(e);
  }
  freeSymbolParser(symbolParser);
}

#endif // TEST
