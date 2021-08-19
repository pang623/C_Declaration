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

extern Tokenizer *tokenizer;

typedef struct Symbol Symbol;
typedef struct OperatorAttrTable OperatorAttrTable;
typedef struct ArityMemory ArityMemory;
typedef struct SymbolCombination SymbolCombination;
typedef Symbol *(*FuncPtr)(Token *, int *);

#define   isIntegerToken(token)                      (token->type == TOKEN_INTEGER_TYPE)
#define   isIdentifierToken(token)                   (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)                         (token->type == TOKEN_NULL_TYPE)
#define   hasSymbolVariations(symbol)                (operatorIdTable[(symbol->str)[0]].func != NULL)
#define   isToken(symToCheck, symbol)                !(stricmp(symToCheck, symbol->str))

struct Symbol {
  int arity;
  int id;
  Token *token;
  Symbol *child[0];
};

struct OperatorAttrTable {
  int type[4];
  FuncPtr func;
};

struct SymbolCombination {
  char *symbol;
  int type;
  int flag;
};

struct ArityMemory {
  int extraMemory;
};

Symbol *createSymbol(Symbol *symbolInfo);
Symbol *getSymbol(Tokenizer *tokenizer);
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
