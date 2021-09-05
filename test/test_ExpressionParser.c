#ifdef TEST

#include "unity.h"
#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Errors.h"
#include "TokenizerExceptionThrowing.h"
#include "ExpressionParser.h"
#include "CDecl_Errors.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "CustomTestAssertion.h"
#include "Tdop.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

SymbolParser *symbolParser;

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
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("3 + (2 *   4)");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_ASSERT_SYMBOL(ADD, "+", Number("3"), Operator("("), symbol);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Number("2"), Number("4"), symbol->child[1]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
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
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("- 3 - + 2 * -   4");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(SUBTRACT, "-", Operator("-"), Operator("*"), symbol);
    TEST_ASSERT_SYMBOL(MINUS_SIGN, "-", Number("3"), NULL, symbol->child[0]);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Operator("+"), Operator("-"), symbol->child[1]);
    TEST_ASSERT_SYMBOL(PLUS_SIGN, "+", Number("2"), NULL, symbol->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(MINUS_SIGN, "-", Number("4"), NULL, symbol->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
          +
         / \
       ++   2
       /  
      3
*/

void test_expression_given_3_postincrement_plus_2_expect_correctly_parsed(void) {
  Symbol *symbol = NULL;
  //parser should just consume the semicolon
  Tokenizer *tokenizer = createTokenizer("3+++2;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(ADD, "+", Operator("++"), Number("2"), symbol);
    TEST_ASSERT_SYMBOL(INC_AFTER, "++", Number("3"), NULL, symbol->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
             +
         /       \
        -         *
       / \       / \
      ~  ++     --  b
     /     \   /   
    2       a 8   
*/

//parsed as ((~2)-(a++))+((--8)*b)
void test_expression_given_mix_of_prefixes_and_suffixes_expect_correctly_parsed(void) {
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("~ 2 - a++ +  --  8 *  b; expression ends after ;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(ADD, "+", Operator("-"), Operator("*"), symbol);
    TEST_ASSERT_SYMBOL(SUBTRACT, "-", Operator("~"), Operator("++"), symbol->child[0]);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Operator("--"), Identifier("b"), symbol->child[1]);
    TEST_ASSERT_SYMBOL(BIT_NOT, "~", Number("2"), NULL, symbol->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(INC_AFTER, "++", Identifier("a"), NULL, symbol->child[0]->child[1]);
    TEST_ASSERT_SYMBOL(DEC_BEFORE, "--", Number("8"), NULL, symbol->child[1]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
          +
         / \
        i  ++
           /
          j
*/

void test_expression_given_plus_plus_but_not_adjacent_expect_not_parsed_as_inc(void) {
  Symbol *symbol = NULL;
  //parsed as i + (++j), not (i++) + j
  Tokenizer *tokenizer = createTokenizer("i + ++ j");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(ADD, "+", Identifier("i"), Operator("++"), symbol);
    TEST_ASSERT_SYMBOL(INC_BEFORE, "++", Identifier("j"), NULL, symbol->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
          -
         / \
        i   -
           /
          +
         /
        j
*/

void test_expression_given_minus_minus_but_not_adjacent_expect_not_parsed_as_dec(void) {
  Symbol *symbol = NULL;
  //parsed as i - (-(+j)), not (i--) + j
  Tokenizer *tokenizer = createTokenizer("i - - + j");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(SUBTRACT, "-", Identifier("i"), Operator("-"), symbol);
    TEST_ASSERT_SYMBOL(MINUS_SIGN, "-", Operator("+"), NULL, symbol->child[1]);
    TEST_ASSERT_SYMBOL(PLUS_SIGN, "+", Identifier("j"), NULL, symbol->child[1]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
          +
         / \
        a   (
            /
          <<=
          /  \
         b    *
             / \
            c   d
*/

//testing right associativity (assignment operators are of right assc.)
void test_expression_given_expression_with_assignment_operator_expect_ast_created_correctly(void) {
  Symbol *symbol = NULL;
  //parsed as a + (b <<= (c * d))
  //which means, a + (b = (b << (c * d)))
  Tokenizer *tokenizer = createTokenizer("a + ( b <<= c * d )");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(ADD, "+", Identifier("a"), Operator("("), symbol);
    TEST_ASSERT_SYMBOL(L_SHIFT_ASSIGN, "<<=", Identifier("b"), Operator("*"), symbol->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Identifier("c"), Identifier("d"), symbol->child[1]->child[0]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
          =
        /   \
       [     +
      / \   / \
   arr  10 10  b
*/

//testing right associativity (assignment operators are of right assc.)
void test_expression_given_expression_with_array_expect_ast_created_correctly(void) {
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("arr[10] = 10+b");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(ASSIGNMENT, "=", Operator("["), Operator("+"), symbol);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("arr"), Number("10"), symbol->child[0]);
    TEST_ASSERT_SYMBOL(ADD, "+", Number("10"), Identifier("b"), symbol->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_with_single_number_expect_ast_created_correctly(void) {
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("3");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    //Test tree created is correct in order
    TEST_ASSERT_SYMBOL(NUMBER, "3", NULL, NULL, symbol);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_3_and_2_expect_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol = NULL;
  Tokenizer *tokenizer = createTokenizer("3 2");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_expression_with_missing_operator_expect_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol = NULL;
  //parsed as (a++)b, missing operator before b
  Tokenizer *tokenizer = createTokenizer("a++b");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_binary_only_operator_used_as_unary_expect_error_syntax_is_thrown(void) {
  Symbol *symbol = NULL;
  //"/" cannot be used as unary operator
  Tokenizer *tokenizer = createTokenizer("5+ /6");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_unary_only_operator_used_as_binary_expect_error_syntax_is_thrown(void) {
  Symbol *symbol = NULL;
  //"~" cannot be used as binary operator
  Tokenizer *tokenizer = createTokenizer("5%6 + 3~9");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
/*
void test_expression_given_missing_operand_for_binary_operator_expect_error_missing_operand_is_thrown(void) {
  Symbol *symbol = NULL;
  //binary ">>=" is missing an operand
  Tokenizer *tokenizer = createTokenizer("a+-b>>= ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
*/
void test_expression_given_missing_operand_for_unary_operator_expect_error_missing_operand_is_thrown(void) {
  Symbol *symbol = NULL;
  //unary "-" is missing an operand
  Tokenizer *tokenizer = createTokenizer("3+5 * -");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_open_parent_used_as_infix_expect_error_syntax_is_thrown(void) {
  Symbol *symbol = NULL;
  //"(" used as infix, invalid
  Tokenizer *tokenizer = createTokenizer("3 (a");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_missing_closing_parent_expect_error_wrong_symbol_is_thrown(void) {
  Symbol *symbol = NULL;
  //missing closing parent
  Tokenizer *tokenizer = createTokenizer("a*(3+2");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_WRONG_SYMBOL, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
/*
void test_expression_given_a_shorthand_left_shift_but_used_as_prefix_expect_error_syntax_is_thrown(void) {
  Symbol *symbol = NULL;
  //"<<=" used as prefix, invalid
  Tokenizer *tokenizer = createTokenizer(" 5 *<<=3");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_a_shorthand_right_shift_but_right_operand_is_not_a_number_or_identifier_expect_error_syntax_is_thrown(void) {
  Symbol *symbol = NULL;
  //symbol ")" is used as right operand for ">>=", and it is also not an unary operator, thus is invalid
  Tokenizer *tokenizer = createTokenizer(" (a>>=) + 3; ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_an_EOL_but_with_no_operands_in_front_of_it_expect_error_missing_operand_is_thrown(void) {
  Symbol *symbol = NULL;
  //";" is expected to have operands in front of it
  Tokenizer *tokenizer = createTokenizer(" ;a+ b");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
*/
void test_expression_given_an_expression_but_with_unknown_symbol_expect_error_invalid_symbol_is_thrown(void) {
  Symbol *symbol = NULL;
  //"#" is an invlaid symbol in expression
  Tokenizer *tokenizer = createTokenizer(" 3# 2");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_an_expression_but_unsupported_symbol_expect_error_invalid_symbol_is_thrown(void) {
  Symbol *symbol = NULL;
  //"#" is an invlaid symbol in expression
  Tokenizer *tokenizer = createTokenizer(" a*b,c");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = expression(0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
    free(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

#endif // TEST
