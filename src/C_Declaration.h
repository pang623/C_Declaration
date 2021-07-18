#ifndef C_DECLARATION_H
#define C_DECLARATION_H

#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CDecl_Errors.h"

//Symbol IDs
#define   ADD         0
#define   MINUS       1
#define   MULTIPLY    2
#define   DIVIDE      3
#define   TILDE       4
#define   NUMBER      5
#define   VARIABLE    6

//Arity
#define   PREFIX      100
#define   INFIX       200
#define   SUFFIX      300
#define   IDENTITY    400

//Macros
#define   getInfixLBP(token)          symbolTable[getSymbolId(token)].infixLBP
#define   getInfixRBP(token)          symbolTable[getSymbolId(token)].infixRBP
#define   getPrefixRBP(token)         symbolTable[getSymbolId(token)].prefixRBP

#define   isIntegerToken(token)       (token->type == TOKEN_INTEGER_TYPE)
#define   isFloatToken(token)         (token->type == TOKEN_FLOAT_TYPE)
#define   isIdentifierToken(token)    (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)          (token->type == TOKEN_NULL_TYPE)

typedef struct Symbol Symbol;
typedef struct SymbolAttrTable SymbolAttrTable;
typedef Symbol *(*nudFuncPtr)();
typedef Symbol *(*ledFuncPtr)(Symbol *left);

struct Symbol {
  Symbol *left;
  Symbol *right;
  Token *token;
  int arity;
};

struct SymbolAttrTable {
  int prefixRBP;
  int infixRBP;
  int infixLBP;
  nudFuncPtr nud;
  ledFuncPtr led;
};

Symbol *expression(int rbp);
Symbol *prefixNud();
Symbol *infixLed(Symbol *left);
Symbol *identityNud();
Symbol *errorNud();
Symbol *errorLed(Symbol *left);
Symbol *identityLed(Symbol *left);
Symbol *createSymbol(Symbol *leftChild, Token *token, int arity, Symbol *rightChild);
Token *peek(Tokenizer *tokenizer);
int getSymbolId(Token *token);
void freeSymbol(Symbol *symbol);

#endif // C_DECLARATION_H
