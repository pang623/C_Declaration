#include "Tdop.h"

void verifyIsSymbolInTable(SymbolParser *symbolParser, Symbol *symbol) {
  if(nudOf(symbol) == NULL && ledOf(symbol) == NULL)
    throwException(ERR_INVALID_SYMBOL, symbol->token, 0,
    "'%s' is not supported here", symbol->token->str);
}

Symbol *tdop(int rbp, SymbolAttrTable *table) {
  //SymbolParser *parser = symbolParser;
  Symbol *left, *symbol, *symbolCheck;
  setSymbolTable(symbolParser, table);
  symbol = getSymbol(symbolParser);
  verifyIsSymbolInTable(symbolParser, symbol);
  left = nudOf(symbol)(symbol);
  while(rbp < getInfixLBP((symbolCheck = peekSymbol(symbolParser)))) {
    symbol = getSymbol(symbolParser);
    setSymbolTable(symbolParser, table);
    verifyIsSymbolInTable(symbolParser, symbol);
    left = ledOf(symbol)(symbol, left);
  }
  verifyIsSymbolInTable(symbolParser, symbolCheck);
  return left;
}
