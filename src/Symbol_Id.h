#ifndef SYMBOL_ID_H
#define SYMBOL_ID_H

/*-------------Symbol IDs---------------*/

typedef enum {
  UNKNOWN,    //For symbols not used in C language
  //Identity
  IDENTIFIER,
  NUMBER,
  
  //Arithmetic Operators
  ADD,
  PLUS_SIGN,
  SUBTRACT,
  MINUS_SIGN,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  INC_BEFORE,
  INC_AFTER,
  DEC_BEFORE,
  DEC_AFTER,

  //Bitwise Operators
  BIT_NOT,
  BIT_AND,
  BIT_OR,
  BIT_XOR,
  L_SHIFT,
  R_SHIFT,

  //Logical Operators
  LOGI_AND,
  LOGI_OR,
  LOGI_NOT,

  //Relational operators
  LESSER,
  GREATER,
  LESS_OR_EQUAL,
  GREATER_OR_EQUAL,
  EQUALITY,
  NOT_EQUAL,

  //Assignment operators (shorthands)
  ASSIGNMENT,
  ADD_ASSIGN,
  SUBT_ASSIGN,
  MUL_ASSIGN,
  DIV_ASSIGN,
  MOD_ASSIGN,
  AND_ASSIGN,
  OR_ASSIGN,
  XOR_ASSIGN,
  L_SHIFT_ASSIGN,
  R_SHIFT_ASSIGN,

  //Misc. Operators
  EOL,
  OPEN_PARENT,
  CLOSE_PARENT,
  OPEN_SQR,
  CLOSE_SQR,
  COMMA,

  //For CDecl
  POINTER,    //"*"
  FUNCTION,   //"("
  
  /*--------------------------------------------------Do not put any symbol ID beyond this line--------------------------------------------------*/
  LAST_SYMBOL,
  
}SymbolID;

#endif // SYMBOL_ID_H
