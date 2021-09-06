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
    //str = translate("int arr[3];");
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
    //str = translate("DOUBLE a[2][3];");
    TEST_ASSERT_EQUAL_STRING("a is array of 2 of array of 3 of DOUBLE", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
}
/*
void test_expression_given_an_float_ptr_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Try {
    str = translate("float *f   ");
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(str);
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
*/
#endif // TEST
