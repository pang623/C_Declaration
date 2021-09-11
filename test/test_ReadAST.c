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
#include "Arithmetic.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

SymbolParser *symbolParser;

void test_readAST_given_an_array_C_declaration_expect_read_out_correctly(void) {
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
    freeException(e);
  }
  free(str);
}

void test_readAST_given_an_twoD_array_C_declaration_expect_read_out_correctly(void) {
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
    freeException(e);
  }
  free(str);
}

void test_readAST_given_an_float_ptr_C_declaration_expect_read_out_correctly(void) {
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
    freeException(e);
  }
  free(str);
}

void test_readAST_given_an_array_of_char_ptr_C_declaration_expect_read_out_correctly(void) {
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
    freeException(e);
  }
  free(str);
}

void test_readAST_given_a_mixed_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  //has function pointer, array, function with no args, and pointer
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
    freeException(e);
  }
  free(str);
}

void test_readAST_given_function_declaration_with_1_args_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("int func(char a); ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("func is function taking in (a is char) returning int", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_readAST_given_function_declaration_with_2_args_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("Symbol *led(Symbol *symbol, Symbol *left); ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("led is function taking in (symbol is pointer to Symbol,left is pointer to Symbol) returning pointer to Symbol", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_readAST_given_function_declaration_with_multiple_args_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("Avengers *asgard(God *Loki , God *Thor, God *evilCharacter[2][5]);");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("asgard is function taking in (Loki is pointer to God,Thor is pointer to God,evilCharacter is array of \
2 of array of 5 of pointer to God) returning pointer to Avengers", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_readAST_given_function_declaration_with_double_ptr_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("int (*(*foo)(char **bar))[3]");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("foo is pointer to function taking in (bar is pointer to pointer to char) returning pointer to array of 3 of int", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_isExpressionReducible_given_expression_with_identifiers_expect_return_false() {
  Tokenizer *tokenizer = createTokenizer("c+b*a");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = expression(0);
  
  int result = isExpressionReducible(AST);
  TEST_ASSERT_EQUAL(0, result);
  
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
}

void test_isExpressionReducible_given_expression_with_no_identifiers_expect_return_true() {
  Tokenizer *tokenizer = createTokenizer("3 >> 2");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = expression(0);
  
  int result = isExpressionReducible(AST);
  TEST_ASSERT_EQUAL(1, result);
  
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
}

void test_convertIntToStr_given_a_number_expect_str_returned() {
  int num = 1359;
  char *str = convertIntToStr(num);
  
  TEST_ASSERT_EQUAL_STRING("1359", str);
  free(str);
}

void test_integerEvaluate_given_an_expression_with_no_identifiers_expect_expression_is_evaluated() {
  Tokenizer *tokenizer = createTokenizer("-(3 << 2) / -2");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = expression(0);
  
  int result = integerEvaluate(AST);
  TEST_ASSERT_EQUAL(6, result);
  
  freeSymbol(AST);
  freeSymbolParser(symbolParser);
}

void test_readAST_given_an_array_C_declaration_and_array_subscripted_with_expression_no_identifiers_expect_array_size_is_evaluated(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("float arr[3- -2*4];");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("arr is array of 11 of float", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_readAST_given_an_array_C_declaration_and_array_subscripted_with_expression_has_identifiers_expect_array_size_not_evaluated(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("float arr[a*4+2];");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *AST = statement();
  Try {
    str = readAST(AST, createString(""));
    TEST_ASSERT_EQUAL_STRING("arr is array of a*4+2 of float", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

void test_readAST_given_a_complex_C_declaration_expect_read_out_correctly(void) {
  char *str = NULL;
  Tokenizer *tokenizer = createTokenizer("char (*(**(*(*(*x(char *str))[(4*7)%5])()))(int num, double *dNum[3]))(Symbol *info[a*9], int *(*b(double d)), INT (*c(int z, char (*func)(int w)))(int e))");
  symbolParser = createSymbolParser(tokenizer);
  
  Try {
    Symbol *AST = statement();
    str = readAST(AST, createString(""));
    //TEST_ASSERT_EQUAL_STRING("x is function taking in (str is pointer to char) \
//returning pointer to array of 5 of pointer to function () returning pointer to function taking in (num is int) returning pointer to pointer to char", str);
    TEST_ASSERT_EQUAL_STRING("x is function taking in (str is pointer to char) returning pointer to array of 3 of pointer to function () \
returning pointer to pointer to pointer to function taking in (num is int,dNum is array of 3 of pointer to double) returning pointer to \
function taking in (info is array of a*9 of pointer to Symbol,b is function taking in (d is double) returning pointer to pointer to int,c \
is function taking in (z is int,func is pointer to function taking in (w is int) returning char) returning pointer to function taking in (e is int) returning INT) returning char", str);
    printf("%s", str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  free(str);
}

#endif // TEST
