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

void test_expression_given_an_array_declaration_expect_correctly_parsed(void) {
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

#endif // TEST
