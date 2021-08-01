#ifndef SYMBOL_H
#define SYMBOL_H

#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "Symbol_Id.h"
#include "Arity.h"
#include "Exception.h"
#include "CDecl_Errors.h"

extern Tokenizer *tokenizer;

typedef struct Symbol Symbol;
typedef struct OperatorAttrTable OperatorAttrTable;
typedef struct ArityMemory ArityMemory;
typedef Symbol *(*FuncPtr)(Token *);

#define   isIntegerToken(token)                      (token->type == TOKEN_INTEGER_TYPE)
#define   isIdentifierToken(token)                   (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)                         (token->type == TOKEN_NULL_TYPE)
#define   isSymbolSameAndAdjacent(symbol, token)     ((symbol->str)[0] == (nextSymbol->str)[0] && nextSymbol->startColumn == symbol->startColumn + 1)
#define   hasSymbolVariations(symbol)                (operatorIdTable[(symbol->str)[0]].func != NULL)
#define   isSymbol(symToCheck, symbol)               !(stricmp(symToCheck, symbol->token->str))
#define   isToken(strToCheck, token)                 !(stricmp(strToCheck, token->str))

struct Symbol {
  int arity;
  int id;
  Token *token;
  Symbol *child[0];
};

struct OperatorAttrTable {
  int type[2];
  FuncPtr func;
};

struct ArityMemory {
  int extraMemory;
};

Symbol *createSymbol(Symbol *symbolInfo);
Symbol *getSymbol(Tokenizer *tokenizer);
Symbol *_getSymbol(Tokenizer *tokenizer);
Symbol *peekSymbol(Tokenizer *tokenizer);
Symbol *checkDoubleSameChar(Token *symbol);
void freeSymbol(void *symbol);
char *createString(char *str);
Symbol *peekStack(DoubleLinkedList *stack);
void pushStack(DoubleLinkedList *stack, Symbol *symbol);
Symbol *popStack(DoubleLinkedList *stack);
void verifyIsSymbolThenConsume(char *symToCheck, Symbol *symbol);

#endif // SYMBOL_H
