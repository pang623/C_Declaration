#ifndef SYMBOL_H
#define SYMBOL_H

#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Symbol_Id.h"
#include "Arity.h"

extern Tokenizer *tokenizer;

typedef struct Symbol Symbol;
typedef struct OperatorAttrTable OperatorAttrTable;
typedef Symbol *(*FuncPtr)(Token *);

#define   isIntegerToken(token)                      (token->type == TOKEN_INTEGER_TYPE)
#define   isIdentifierToken(token)                   (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)                         (token->type == TOKEN_NULL_TYPE)
#define   isSymbolSameAndAdjacent(symbol, token)     ((symbol->str)[0] == (nextSymbol->str)[0] && nextSymbol->startColumn == symbol->startColumn + 1)
//#define   isCloseParentToken(token)   ((token->str)[0] == ')')

struct Symbol {
  Symbol *left;
  Symbol *right;
  Token *token;
  int arity;
  int id;
};

struct OperatorAttrTable {
  int type[2];
  FuncPtr func;
};

Symbol *createSymbol(Symbol *symbolInfo);
Symbol *getSymbol(Tokenizer *tokenizer);
Symbol *_getSymbol(Tokenizer *tokenizer);
Symbol *peekSymbol(Tokenizer *tokenizer);
Symbol *checkDoubleChar(Token *symbol);
void freeSymbol(void *symbol);
char *createString(char *str);
Symbol *peekStack(DoubleLinkedList *stack);
void pushStack(DoubleLinkedList *stack, Symbol *symbol);
Symbol *popStack(DoubleLinkedList *stack);

#endif // SYMBOL_H
