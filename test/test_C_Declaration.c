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
#include "CustomTestAssertion.h"
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
extern DoubleLinkedList *symbolStack;

/*
          +
         / \
        3   (
           /
          *
         / \
        2   4
*/

void test_expression_given_3_plus_2_times_4_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3 +(2 *   4)");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    
    TEST_ASSERT_SYMBOL(ADD, "+", Number("3"), Operator("("), symbol);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Number("2"), Number("4"), symbol->child[1]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

/*
          -
         / \
        -   *
       /   / \
      3   +   -
         /   /
        2   4
*/

void test_expression_given_NEG3_MINUS_2_times_NEG4_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("-3 -+2 *-   4");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(SUBTRACT, "-", Operator("-"), Operator("*"), symbol);
    TEST_ASSERT_SYMBOL(MINUS_SIGN, "-", Number("3"), NULL, symbol->child[0]);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Operator("+"), Operator("-"), symbol->child[1]);
    TEST_ASSERT_SYMBOL(PLUS_SIGN, "+", Number("2"), NULL, symbol->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(MINUS_SIGN, "-", Number("4"), NULL, symbol->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_3_postincrement_plus_2_expect_correctly_parsed(void) {
  Symbol *symbol;
  //parser should just consume the semicolon
  tokenizer = createTokenizer("3+++2;");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("++", symbol->child[0]->token->str);
    TEST_ASSERT_EQUAL(SUFFIX, symbol->child[0]->arity);
    TEST_ASSERT_EQUAL_STRING("3", symbol->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->child[0]->child[0]->arity);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("2", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->child[1]->arity);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

//parsed as ((~2)-(a++))+((--8)*b)
void test_expression_given_mix_of_prefixes_and_suffixes_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("~2-a+++--8*b");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL_STRING("-", symbol->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("~", symbol->child[0]->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[0]->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("2", symbol->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("++", symbol->child[0]->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("a", symbol->child[0]->child[1]->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]->child[1]);
    TEST_ASSERT_EQUAL_STRING("*", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("--", symbol->child[1]->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("8", symbol->child[1]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("b", symbol->child[1]->child[1]->token->str);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

//parsed as ((~2)-(a++))+((--8)*b)
void test_expression_given_mix_of_prefixes_and_suffixes_with_parentheses_expect_correctly_parsed(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("((~2)-(a++))+((--8)*b)");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("-", symbol->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("~", symbol->child[0]->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("2", symbol->child[0]->child[0]->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[0]->child[0]->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("++", symbol->child[0]->child[0]->child[1]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("a", symbol->child[0]->child[0]->child[1]->child[0]->child[0]->token->str);
    
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("*", symbol->child[1]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[1]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("--", symbol->child[1]->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("8", symbol->child[1]->child[0]->child[0]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("b", symbol->child[1]->child[0]->child[1]->token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_plus_plus_but_not_adjacent_expect_not_parsed_as_inc(void) {
  Symbol *symbol;
  //parsed as i + (++j), not (i++) + j
  tokenizer = createTokenizer("i+ ++j");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL_STRING("i", symbol->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("++", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("j", symbol->child[1]->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_minus_minus_but_not_adjacent_expect_not_parsed_as_dec(void) {
  Symbol *symbol;
  //parsed as i - (-(+j)), not (i--) + j
  tokenizer = createTokenizer("i- -+j");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("-", symbol->token->str);
    TEST_ASSERT_EQUAL_STRING("i", symbol->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("-", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("+", symbol->child[1]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("j", symbol->child[1]->child[0]->child[0]->token->str);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[0]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

//testing right associativity (assignment operators are of right assc.)
void test_expression_given_expression_with_assignment_operator_expect_ast_created_correctly(void) {
  Symbol *symbol;
  //parsed as a + (b <<= (c + d))
  //which means, a + (b = (b << (c + d)))
  tokenizer = createTokenizer("a +( b<<= c*d)");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL_STRING("a", symbol->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("(", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("<<=", symbol->child[1]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("b", symbol->child[1]->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("*", symbol->child[1]->child[0]->child[1]->token->str);
    TEST_ASSERT_EQUAL_STRING("c", symbol->child[1]->child[0]->child[1]->child[0]->token->str);
    TEST_ASSERT_EQUAL_STRING("d", symbol->child[1]->child[0]->child[1]->child[1]->token->str);
    TEST_ASSERT_NULL(symbol->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[0]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]->child[0]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]->child[0]->child[1]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]->child[1]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]->child[1]->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_3_and_2_expect_error_expected_operator_is_thrown(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3 2");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTED_OPERATOR, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_expression_with_missing_operator_expect_error_expected_operator_is_thrown(void) {
  Symbol *symbol;
  //parsed as (a++)b, missing operator before b
  tokenizer = createTokenizer("a++b");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTED_OPERATOR, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_binary_only_operator_used_as_unary_expect_error_syntax_is_thrown(void) {
  Symbol *symbol;
  //"/" cannot be used as unary operator
  tokenizer = createTokenizer("5+ /6");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_unary_only_operator_used_as_binary_expect_error_syntax_is_thrown(void) {
  Symbol *symbol;
  //"~" cannot be used as binary operator
  tokenizer = createTokenizer("5%6 + 3~9");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_missing_operand_for_binary_operator_expect_error_missing_operand_is_thrown(void) {
  Symbol *symbol;
  //binary ">>=" is missing an operand
  tokenizer = createTokenizer("a+-b>>= ");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_missing_operand_for_unary_operator_expect_error_missing_operand_is_thrown(void) {
  Symbol *symbol;
  //unary "-" is missing an operand
  tokenizer = createTokenizer("3+5 * -");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_open_parent_used_as_infix_expect_error_syntax_is_thrown(void) {
  Symbol *symbol;
  //"(" used as infix, invalid
  tokenizer = createTokenizer("3 (a");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_missing_closing_parent_expect_error_wrong_symbol_is_thrown(void) {
  Symbol *symbol;
  //missing closing parent
  tokenizer = createTokenizer("a*(3+2");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_WRONG_SYMBOL, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_a_closing_parent_but_missing_opening_parent_expect_error_parse_error_is_thrown(void) {
  Symbol *symbol;
  //missing open parent
  tokenizer = createTokenizer(" 5 * 3) -1");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_PARSE_ERROR, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_a_shorthand_left_shift_but_used_as_prefix_expect_error_syntax_is_thrown(void) {
  Symbol *symbol;
  //"<<=" used as prefix, invalid
  tokenizer = createTokenizer(" 5 *<<=3");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

void test_expression_given_a_shorthand_right_shift_but_used_as_suffix_expect_error_syntax_is_thrown(void) {
  Symbol *symbol;
  //">>=" used as suffix, invalid
  tokenizer = createTokenizer(" (a>>=) + 3");
  symbolStack = linkedListCreateList();
  Try {
    symbol = parse(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  linkedListFreeList(symbolStack, freeSymbol);
  freeTokenizer(tokenizer);
}

/*
//this is evaluated to (3++)++ + 2, which is invalid
void test_expression_given_3_doublepostincrement_plus_2_expect_ERR_SYNTAX_thrown(void) {
  Symbol *symbol;
  tokenizer = createTokenizer("3+++++2");
  Try{
    symbol = parse(0);
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
    symbol = parse(0);
    //Test tree created is correct in order
    TEST_ASSERT_EQUAL_STRING("+", symbol->token->str);
    TEST_ASSERT_EQUAL(INFIX, symbol->arity);
    TEST_ASSERT_EQUAL_STRING("++", symbol->child[0]->token->str);
    TEST_ASSERT_EQUAL(SUFFIX, symbol->child[0]->arity);
    TEST_ASSERT_EQUAL_STRING("3", symbol->child[0]->child[0]->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->child[0]->child[0]->arity);
    TEST_ASSERT_NULL(symbol->child[0]->child[1]);
    TEST_ASSERT_EQUAL_STRING("2", symbol->child[1]->token->str);
    TEST_ASSERT_EQUAL(IDENTITY, symbol->child[1]->arity);
    TEST_ASSERT_NULL(symbol->child[1]->child[0]);
    TEST_ASSERT_NULL(symbol->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeTokenizer(tokenizer);
}
*/
#endif // TEST
