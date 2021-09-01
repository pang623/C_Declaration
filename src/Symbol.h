#ifndef SYMBOL_H
#define SYMBOL_H

#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "Exception.h"
#include "CDecl_Errors.h"
#include "SymbolCombination.h"
#include "SymbolAttrTable.h"

typedef struct OperatorAttrTable OperatorAttrTable;
typedef struct ArityMemory ArityMemory;
typedef struct SymbolCombination SymbolCombination;
typedef struct SymbolParser SymbolParser;
typedef Symbol *(*FuncPtr)(Token *, int *);

#define   isIntegerToken(token)                      (token->type == TOKEN_INTEGER_TYPE)
#define   isFloatToken(token)                        (token->type == TOKEN_FLOAT_TYPE)
#define   isIdentifierToken(token)                   (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)                         (token->type == TOKEN_NULL_TYPE)
#define   hasSymbolVariations(symbol)                (operatorIdTable[(symbol->str)[0]].func != NULL)
#define   isToken(symToCheck, symbol)                !(stricmp(symToCheck, symbol->str))

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
/*
struct ArityMemory {
  int extraMemory;
};*/

struct SymbolParser {
  Tokenizer *tokenizer;
  DoubleLinkedList *symbolStack;
  SymbolAttrTable *symbolTable;
};

Symbol *createSymbol(Symbol *symbolInfo);
SymbolParser *createSymbolParser(Tokenizer *tokenizer);
void freeSymbolParser(SymbolParser *symbolParser);
void setSymbolTable(SymbolParser *symbolParser, SymbolAttrTable *table);
Symbol *getSymbol(SymbolParser *symbolParser);
Symbol *_getSymbol(Tokenizer *tokenizer);
Symbol *peekSymbol(Tokenizer *tokenizer);
void freeSymbol(void *symbol);
char *createString(char *str);
Symbol *peekStack(DoubleLinkedList *stack);
void pushStack(DoubleLinkedList *stack, Symbol *symbol);
Symbol *popStack(DoubleLinkedList *stack);
void verifyIsNextSymbolThenConsume(Tokenizer *tokenizer, int symbolId, char *expectedSym);
Token *processToken(Token *symbol, int option);
Symbol *processSymbol(Token *symbol, int *flag, int option, int type);
int isCorrectSymbolAndAdjacent(Token *symbol, Token *nextSymbol, char *symToCheck);
Symbol *checkEqualAsLastChar(Token *symbol, int *flag);
Symbol *checkDoubleSameChar(Token *symbol, int *flag);
Symbol *checkDoubleSameCharWithEqual(Token *symbol, int *flag);

#endif // SYMBOL_H
