#include "CDecl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//If both nud and led are NULL, it indicates that that particular symbol is not supported
SymbolAttrTable CDeclSymbolTable[LAST_SYMBOL] = {
  //[SYMBOLID]         =   {prefixRBP, infixRBP, infixLBP,      nud,           led}
  [IDENTIFIER]         =   { NIL,  NIL,  NIL,           identityNud,   identityLed},
  [MULTIPLY]           =   { 140,  NIL,  NIL,            pointerNud,    pointerLed},
  [OPEN_PARENT]        =   {   0,    0,  150,           groupingNud,       funcLed},
  [CLOSE_PARENT]       =   {   0,    0,    0,              errorNud,          NULL},
  [OPEN_SQR]           =   { NIL,    0,  150,              errorNud, sqrBracketLed},
  [CLOSE_SQR]          =   {  0,     0,    0,              errorNud,          NULL},
  [COMMA]              =   {  0,     0,    0,              errorNud,          NULL},
  [EOL]                =   {  0,     0,    0,     missingOperandNud,          NULL},
};

FudFuncPtr keywordTable[] = {
  //[KEYWORD_TYPE] = function pointer
  [TYPE]           =       cDecl,
  [FOR]            =      forFud,
  [WHILE]          =    whileFud,
  [IF]             =       ifFud,
  [CASE]           =     caseFud,
  [CONTINUE]       = continueFud,
  [BREAK]          =    breakFud,
};

//These will throw error at the moment, it can be implemented in the future
Symbol *forFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *whileFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *ifFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *caseFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *continueFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *breakFud(int rbp) {
  Symbol *symbol = getSymbol(symbolParser);
  throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
  "Expecting a data type keyword here", symbol->token->str);
}

Symbol *groupingNud(Symbol *symbol, Symbol *left) {
  symbol->arity = PREFIX;
  symbol->child[0] = tdop(0, CDeclSymbolTable);
  verifyIsNextSymbolThenConsume(CLOSE_PARENT, ")");
  return symbol;
}

Symbol *funcLed(Symbol *symbol, Symbol *left) {
  Symbol *AST, *temp, *newAST, *next;
  newAST = next = AST = temp = NULL;
  symbol->id = FUNCTION;
  symbol->child[0] = left;
  while(1) {
    if((peekSymbol(symbolParser))->id == CLOSE_PARENT) {
      if(next != NULL && next->id == COMMA) {
        freeSymbol(left);
        throwException(ERR_EXPECING_CDECL, peekSymbol(symbolParser)->token, 0,
        "Expecting a C declaration here");
      }
      break;
    }
    AST = cDecl(0);
    if((next = peekSymbol(symbolParser))->id == COMMA) {
      temp = getSymbol(symbolParser);
      temp->child[0] = AST;
      newAST = combineAST(temp, newAST);
    }else if(next->id == CLOSE_PARENT) {
      newAST = combineAST(AST, newAST);
      break;
    }else {
      freeSymbol(left);
      throwException(ERR_SYNTAX, next->token, 0,
      "Must be a ',' or ')' here");
    }
  }
  verifyIsNextSymbolThenConsume(CLOSE_PARENT, ")");
  symbol->child[1] = newAST;
  return symbol;
}

//grows AST to the right subtree
Symbol *combineAST(Symbol *AST, Symbol *oldAST) {
  if(oldAST == NULL)
    return oldAST = AST;
  oldAST->child[1] = combineAST(AST, oldAST->child[1]);
  return oldAST;
}

Symbol *pointerLed(Symbol *symbol, Symbol *left) {
  freeSymbol(left);
  throwException(ERR_SYNTAX, symbol->token, 0,
  "Pointer cannot be used here");
}

Symbol *pointerNud(Symbol *symbol, Symbol *left) {
  symbol->arity = PREFIX;
  symbol->id = POINTER;
  symbol->child[0] = tdop(getPrefixRBP(symbol), CDeclSymbolTable);
  return symbol;
}

Symbol *cDecl(int rbp) {
  int i;
  int *type = &i;
  Symbol *symbol = getSymbol(symbolParser);
  if((!isSymbolKeywordThenGetType(symbol, type, 1)) || (*type != TYPE))
    throwException(ERR_KEYWORD_DATA_TYPE, symbol->token, 0,
    "Expecting a data type keyword here", symbol->token->str);
  symbol->id = TYPE;
  symbol->arity = IDENTITY;
  symbol->child[0] = tdop(rbp, CDeclSymbolTable);
  return symbol;
}

Symbol *statement() {
  int i;
  int *type = &i;
  Symbol *left;
  Symbol *symbol = peekSymbol(symbolParser);
  if(isSymbolKeywordThenGetType(symbol, type, 1))
    left = fudOf(*type)(0);
  else
    left = expression(0);
  verifyIsNextSymbolThenConsume(EOL, ";");
  return left;
}
