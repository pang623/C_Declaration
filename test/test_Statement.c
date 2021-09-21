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
#include "Arithmetic.h"
#include "Statement.h"
#include "Strings.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

SymbolParser *symbolParser;

void test_statement_given_cdecl_expect_able_to_be_parsed() {
  Symbol *symbol = NULL;
  Symbol *testSymbol = NULL;
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
    freeException(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}

void test_statement_given_expression_expect_able_to_be_parsed() {
  Symbol *symbol = NULL;
  Symbol *testSymbol = NULL;
  Tokenizer *tokenizer = createTokenizer("3 - --b");
  symbolParser = createSymbolParser(tokenizer);
  Try {
    symbol = statement();
    TEST_ASSERT_SYMBOL(SUBTRACT, "-", Number("3"), Operator("--"), symbol);
    TEST_ASSERT_SYMBOL(DEC_BEFORE, "--", Identifier("b"), NULL, testSymbol = symbol->child[1]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
/*
void test_statement_given_c_declaration_but_terminated_with_comma_expect_ERR_WRONG_SYMBOL_is_thrown(void) {
  Tokenizer *tokenizer = createTokenizer("int a(int b), ");
  symbolParser = createSymbolParser(tokenizer);
  Symbol *symbol = NULL;
  Try {
    symbol = statement();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_WRONG_SYMBOL, e->errorCode);
    freeException(e);
  }
  freeSymbol(symbol);
  freeSymbolParser(symbolParser);
}
*/
#endif // TEST
