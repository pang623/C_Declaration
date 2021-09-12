#include "Statement.h"

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