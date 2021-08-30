#ifdef TEST

#include "unity.h"
#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Errors.h"
#include "TokenizerExceptionThrowing.h"
#include "ExpressionParser.h"
#include "CDecl.h"
#include "CDecl_Errors.h"
#include "Symbol.h"
#include "SymbolAttrTable.h"
#include "KeywordTable.h"
#include "Symbol_Id.h"
#include "KeywordType.h"
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

SymbolParser *symbolParser;

/*
          int
         /
        [   
       / \
     arr  3
*/

void test_expression_given_an_array_C_declaration_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int arr[3];");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "int", Operator("["), NULL, symbol);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("arr"), Number("3"), symbol->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_an_array_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Try {
    str = translate("int a[3];");
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

/*
          DOUBLE
         /
        [   
       / \
      [   3
     / \
    a   2
*/

void test_expression_given_an_twoD_array_C_declaration_expect_correctly_parsed(void) {
  Symbol *symbol;
  //In this case DOUBLE is seen as a user defined data type, not the 'double' type, so will be accepted as well
  Tokenizer *tokenizer = createTokenizer("DOUBLE a[2][3];");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "DOUBLE", Operator("["), NULL, symbol);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Operator("["), Number("3"), symbol->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("a"), Number("2"), symbol->child[0]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_an_twoD_array_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Try {
    str = translate("DOUBLE a[2][3];");
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

/*
          char
         /
        *   
       /
      [
     / \
    b   10
*/

void test_expression_given_an_array_of_char_ptr_C_declaration_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("char   * b[10]  ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "char", Operator("*"), NULL, symbol);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("["), NULL, symbol->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("b"), Number("10"), symbol->child[0]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_an_array_of_char_ptr_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Try {
    str = translate("char   * b[10]  ");
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

/*
          float
         /
        [   
       / \
    arr   +
         / \
        3   *
           / \
          a   4
*/

void test_expression_given_an_array_C_declaration_and_array_subscripted_with_expression_expect_correctly_parsed(void) {
  Symbol *symbol;
  //In this case DOUBLE is seen as a user defined data type, not the 'double' type, so will be accepted as well
  Tokenizer *tokenizer = createTokenizer("float arr[3+a*4];");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "float", Operator("["), NULL, symbol);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("arr"), Operator("+"), symbol->child[0]);
    TEST_ASSERT_SYMBOL(ADD, "+", Number("3"), Operator("*"), symbol->child[0]->child[1]);
    TEST_ASSERT_SYMBOL(MULTIPLY, "*", Identifier("a"), Number("4"), symbol->child[0]->child[1]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_two_separate_variable_names_expect_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol;
  //'a' and 'b' separated, both are names, invalid, variable name can only be one
  Tokenizer *tokenizer = createTokenizer("Symbol a b;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_extra_variable_name_expect_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol;
  //two names declared, invalid, can only have one name
  Tokenizer *tokenizer = createTokenizer("Symbol a[3]extra;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_SYNTAX, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_variable_is_a_number_expect_error_invalid_symbol_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int 3;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_is_variable_but_mixed_with_operators_expect_error_invalid_symbol_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int a +3;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_SYMBOL, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_keyword_is_not_data_type_expect_ERR_KEYWORD_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("while var;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_KEYWORD, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_keyword_is_used_as_variable_name_expect_ERR_ILLEGAL_KEYWORD_USAGE_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("char if;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_KEYWORD_USAGE, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

#endif // TEST
