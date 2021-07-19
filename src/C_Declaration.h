#ifndef C_DECLARATION_H
#define C_DECLARATION_H

#include "Exception.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CDecl_Errors.h"

//Symbol IDs
#define   NUMBER      0
#define   VARIABLE    1
#define   ADD         2
#define   MINUS       3
#define   MULTIPLY    4
#define   DIVIDE      5
#define   MODULUS     6
#define   TILDE       7
#define   NOT         8
#define   INC         9
#define   DEC         10

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
#define   isIdentifierToken(token)    (token->type == TOKEN_IDENTIFIER_TYPE)
#define   isNULLToken(token)          (token->type == TOKEN_NULL_TYPE)
#define   isIncToken(token)           (!(stricmp(token->str, "++")))
#define   isDecToken(token)           (!(stricmp(token->str, "--")))

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
Symbol *prefixNudTwo();
Symbol *infixLedL(Symbol *left);
Symbol *infixLedR(Symbol *left);
Symbol *suffixLed(Symbol *left);
Symbol *identityNud();
Symbol *errorNud();
Symbol *errorLed(Symbol *left);
Symbol *identityLed(Symbol *left);
Symbol *createSymbol(Symbol *leftChild, Token *token, int arity, Symbol *rightChild);
Token *peek(Tokenizer *tokenizer);
int getSymbolId(Token *token);
void freeSymbol(Symbol *symbol);
char *createString(char *str);

#endif // C_DECLARATION_H
