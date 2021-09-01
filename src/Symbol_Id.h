#ifndef SYMBOL_ID_H
#define SYMBOL_ID_H

/*-------------Symbol IDs---------------*/

//Identity
#define   UNKNOWN             0
#define   IDENTIFIER          1
#define   NUMBER              2

//Arithmetic Operators
#define   ADD                 10
#define   PLUS_SIGN           11
#define   SUBTRACT            12
#define   MINUS_SIGN          13
#define   MULTIPLY            14
#define   DIVIDE              15
#define   MODULUS             16
#define   INC_BEFORE          17
#define   INC_AFTER           18
#define   DEC_BEFORE          19
#define   DEC_AFTER           20

//Bitwise Operators
#define   BIT_NOT             21
#define   BIT_AND             22
#define   BIT_OR              23
#define   BIT_XOR             24
#define   L_SHIFT             25
#define   R_SHIFT             26

//Logical Operators
#define   LOGI_AND            30
#define   LOGI_OR             31
#define   LOGI_NOT            32

//Relational operators
#define   LESSER              40
#define   GREATER             41
#define   LESS_OR_EQUAL       42
#define   GREATER_OR_EQUAL    43
#define   EQUALITY            44
#define   NOT_EQUAL           45

//Assignment operators (shorthands)
#define   ASSIGNMENT          50
#define   ADD_ASSIGN          51
#define   SUBT_ASSIGN         52
#define   MUL_ASSIGN          53
#define   DIV_ASSIGN          54
#define   MOD_ASSIGN          55
#define   AND_ASSIGN          56
#define   OR_ASSIGN           57
#define   XOR_ASSIGN          58
#define   L_SHIFT_ASSIGN      59
#define   R_SHIFT_ASSIGN      60

//Misc. Operators
#define   EOL                 70
#define   OPEN_PARENT         71
#define   CLOSE_PARENT        72
#define   OPEN_SQR            73
#define   CLOSE_SQR           74
#define   COMMA               75

//For CDecl
#define   POINTER             76
#define   GROUPING            77
#define   FUNCTION            78

#endif // SYMBOL_ID_H
