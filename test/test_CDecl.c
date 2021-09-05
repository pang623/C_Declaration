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
#include "Symbol_Id.h"
#include "KeywordType.h"
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

/*
          float
         /
        *   
       /
      f
*/

void test_expression_given_an_float_ptr_C_declaration_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("float *f  ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "float", Operator("*"), NULL, symbol);
    TEST_ASSERT_SYMBOL(POINTER, "*", Identifier("f"), NULL, symbol->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
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

/*
         int
         /
        (   
       / 
   func   
*/
  
void test_expression_given_a_function_declaration_with_empty_parameters_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int func(); ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "int", Operator("("), NULL, symbol);
    TEST_ASSERT_SYMBOL(FUNCTION, "(", Identifier("func"), NULL, symbol->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
         int
         /
        (   
       / \
   func   char
          /
         a
*/
  
void test_expression_given_a_function_declaration_with_single_parameter_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int func(char a); ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "int", Operator("("), NULL, symbol);
    TEST_ASSERT_SYMBOL(FUNCTION, "(", Identifier("func"), Identifier("char"), symbol->child[0]);
    TEST_ASSERT_SYMBOL(TYPE, "char", Identifier("a"), NULL, symbol->child[0]->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
               Symbol
               /
              *   
             /
            ( 
           / \
        led   ,
             / \
       Symbol   Symbol
          /       /
         *       *
        /       /
     symbol   left
*/
  
void test_expression_given_a_function_declaration_with_two_parameters_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("Symbol *led(Symbol *symbol, Symbol *left); ");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "Symbol", Operator("*"), NULL, symbol);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("("), NULL, symbol->child[0]);
    TEST_ASSERT_SYMBOL(FUNCTION, "(", Identifier("led"), Operator(","), symbol->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(COMMA, ",", Identifier("Symbol"), Identifier("Symbol"), symbol->child[0]->child[0]->child[1]);
    TEST_ASSERT_SYMBOL(TYPE, "Symbol", Operator("*"), NULL, symbol->child[0]->child[0]->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(TYPE, "Symbol", Operator("*"), NULL, symbol->child[0]->child[0]->child[1]->child[1]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Identifier("symbol"), NULL, symbol->child[0]->child[0]->child[1]->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Identifier("left"), NULL, symbol->child[0]->child[0]->child[1]->child[1]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

/*
             Avengers
               /
              *   
             /
            ( 
           / \
     asgard   ,     
             / \    
          God   , 
          /    / \ 
         *   God  God
        /     /    / 
      Loki   *    *
            /    /
          Thor  [  
               / \ 
              [   5
             /  \
  evilCharacter  2
*/
  
void test_expression_given_a_function_declaration_with_multiple_parameters_expect_correctly_parsed(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("Avengers *asgard(God *Loki , God *Thor, God *evilCharacter[2][5])");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "Avengers", Operator("*"), NULL, symbol);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("("), NULL, symbol->child[0]);
    TEST_ASSERT_SYMBOL(FUNCTION, "(", Identifier("asgard"), Operator(","), symbol->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(COMMA, ",", Identifier("God"), Operator(","), symbol->child[0]->child[0]->child[1]);
    TEST_ASSERT_SYMBOL(COMMA, ",", Identifier("God"), Identifier("God"), symbol->child[0]->child[0]->child[1]->child[1]);
    TEST_ASSERT_SYMBOL(TYPE, "God", Operator("*"), NULL, symbol->child[0]->child[0]->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(TYPE, "God", Operator("*"), NULL, symbol->child[0]->child[0]->child[1]->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(TYPE, "God", Operator("*"), NULL, symbol->child[0]->child[0]->child[1]->child[1]->child[1]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Identifier("Loki"), NULL, symbol->child[0]->child[0]->child[1]->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Identifier("Thor"), NULL, symbol->child[0]->child[0]->child[1]->child[1]->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("["), NULL, symbol->child[0]->child[0]->child[1]->child[1]->child[1]->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Operator("["), Number("5"), symbol->child[0]->child[0]->child[1]->child[1]->child[1]->child[0]->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("evilCharacter"), Number("2"), symbol->child[0]->child[0]->child[1]->child[1]->child[1]->child[0]->child[0]->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
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

void test_expression_given_an_mixed_C_declaration_expect_correctly_parsed(void) {
  Symbol *symbol, *testSymbol;
  Tokenizer *tokenizer = createTokenizer("char (*(*x[3])())[5]");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(TYPE, "char", Operator("["), NULL, symbol);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Operator("("), Number("5"), testSymbol = symbol->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_PARENT, "(", Operator("*"), NULL, testSymbol = testSymbol->child[0]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("("), NULL, testSymbol = testSymbol->child[0]);
    TEST_ASSERT_SYMBOL(FUNCTION, "(", Operator("("), NULL, testSymbol = testSymbol->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_PARENT, "(", Operator("*"), NULL, testSymbol = testSymbol->child[0]);
    TEST_ASSERT_SYMBOL(POINTER, "*", Operator("["), NULL, testSymbol = testSymbol->child[0]);
    TEST_ASSERT_SYMBOL(OPEN_SQR, "[", Identifier("x"), Number("3"), testSymbol = testSymbol->child[0]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
/*
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

void test_expression_given_c_declaration_but_keyword_is_not_data_type_expect_ERR_KEYWORD_DATA_TYPE_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("while var;");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_KEYWORD_DATA_TYPE, e->errorCode);
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

void test_expression_given_c_declaration_pointer_symbol_comes_after_variable_expect_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("double a*[3];");
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

void test_expression_given_c_declaration_but_no_name_expect_ERR_MISSING_OPERAND_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("int *");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_MISSING_OPERAND, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_no_name_expect_ERR_EXPECING_CDECL_is_thrown(void) {
  Symbol *symbol;
  Tokenizer *tokenizer = createTokenizer("Token *getToken(Tokenizer *tokenizer, );");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECING_CDECL, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_declaration_but_expression_in_array_subscript_is_erroneous_ERR_WRONG_SYMBOL_is_thrown(void) {
  Symbol *symbol;
  //expression terminates at ")", expression by right should terminate at "]" in this case, thus error is thrown
  Tokenizer *tokenizer = createTokenizer("int a[c+d)*4]");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_WRONG_SYMBOL, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_function_decl_but_parameters_passed_in_is_not_CDecl_ERR_KEYWORD_DATA_TYPE_is_thrown(void) {
  Symbol *symbol;
  //expression terminates at "]", thus the 
  Tokenizer *tokenizer = createTokenizer("int a(char b, 32)");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_KEYWORD_DATA_TYPE, e->errorCode);
  }
  freeSymbolParser(symbolParser);
}

void test_expression_given_c_function_decl_but_parameters_are_not_separated_with_comma_ERR_SYNTAX_is_thrown(void) {
  Symbol *symbol;
  //separated by "]", throw error
  Tokenizer *tokenizer = createTokenizer("int a(char b ] int a)");
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
*/
#endif // TEST
