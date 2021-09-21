#ifndef SYMBOL_H
#define SYMBOL_H

#include <string.h>
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "SymbolCombination.h"
#include "SymbolAttrTable.h"
#include "KeywordType.h"
#include "Strings.h"

typedef struct OperatorAttrTable OperatorAttrTable;
typedef struct SymbolCombination SymbolCombination;
typedef struct KeywordAttrTable KeywordAttrTable;
typedef struct SymbolParser SymbolParser;

typedef Symbol (*FuncPtr)(Token *, int *);

#define   isIntegerToken(token)                      (token->type == TOKEN_INTEGER_TYPE)
#define   isFloatToken(token)                        (token->type == TOKEN_FLOAT_TYPE)
#define   isIdentifierToken(token)                   (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)                         (token->type == TOKEN_NULL_TYPE)
#define   hasSymbolVariations(symbol)                (operatorIdTable[(symbol->str)[0]].func != NULL)
#define   isToken(symToCheck, symbol)                !(strcmp(symToCheck, symbol->str))

#define   popStack(stack)                            (linkedListRemoveItemFromHead(stack))->data
#define   pushStack(stack, item)                     (linkedListAddItemToHead(item, stack))
#define   peekStack(stack)                           (stack->head->data)

extern SymbolParser *symbolParser;

struct OperatorAttrTable {
  int type[4];
  FuncPtr func;
};

struct SymbolCombination {
  char *symbol;
  int type;
  int flag;
};

struct KeywordAttrTable {
  char *keyword;
  int type;
};

struct SymbolParser {
  Tokenizer *tokenizer;
  DoubleLinkedList *symbolStack;
  SymbolAttrTable *symbolTable;
};

Symbol *createSymbol(Symbol *symbolInfo);
void freeSymbol(void *symbol);
SymbolParser *createSymbolParser(Tokenizer *tokenizer);
void freeSymbolParser(SymbolParser *symbolParser);
void setSymbolTable(SymbolParser *symbolParser, SymbolAttrTable *table);
Symbol *peekSymbol(SymbolParser *symbolParser);
Symbol *getSymbol(SymbolParser *symbolParser);
Symbol *_getSymbol(Tokenizer *tokenizer);
int isNextSymbolThenConsume(SymbolID symbolId);
void verifyIsNextSymbolThenConsume(SymbolID symbolId, char *expectedSym);
Token *processToken(Token *symbol, int option);
Symbol processSymbol(Token *symbol, int *flag, int option, int type);
int isCorrectSymbolAndAdjacent(Token *symbol, Token *nextSymbol, char *symToCheck);
Symbol handleEqualSymbol(Token *symbol, int *flag);
Symbol handleRepeatedAndEqualSymbol(Token *symbol, int *flag);
Symbol handleEqualRepeatedAndBothSymbol(Token *symbol, int *flag);
int isSymbolKeyword(Symbol *symbol, int identifierIsSeenAsKeyword);
int isSymbolKeywordThenGetType(Symbol *symbol, int *type, int identifierIsSeenAsKeyword);

#endif // SYMBOL_H
