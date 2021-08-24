#ifndef KEYWORDTABLE_H
#define KEYWORDTABLE_H

#include "SymbolAttrTable.h"

typedef struct StatementKeywordTable StatementKeywordTable;
typedef Symbol *(*FudFuncPtr)(Symbol *symbol);

struct StatementKeywordTable {
  char *keyword;
  int type;
  FudFuncPtr fud;
};

#endif // KEYWORDTABLE_H
