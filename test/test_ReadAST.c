#ifdef TEST

#include "unity.h"
#include "ReadAST.h"
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
#include "Symbol_Id.h"
#include "KeywordType.h"
#include "Arity.h"
#include "CustomTestAssertion.h"
#include "Tdop.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

SymbolParser *symbolParser;

void test_expression_given_an_array_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("int arr[3];");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    printf("%s", str);
    TEST_ASSERT_EQUAL_STRING("arr is array of 3 of int", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

void test_expression_given_an_twoD_array_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("DOUBLE a[2][3];");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("a is array of 2 of array of 3 of DOUBLE", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

void test_expression_given_an_float_ptr_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("float *f   ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("f is pointer to float", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

void test_expression_given_an_array_of_char_ptr_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("char   * b[10]  ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("b is array of 10 of pointer to char", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

void test_expression_given_an_mixed_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("char (*(*x[3])())[5]");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("x is array of 3 of pointer to function () returning pointer to array of 5 of char", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}

#endif // TEST
