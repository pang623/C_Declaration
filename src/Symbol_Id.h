#ifndef SYMBOL_ID_H
#define SYMBOL_ID_H

/*-------------Symbol IDs---------------*/

//Identity
#define   NUMBER              0
#define   VARIABLE            1

//Arithmetic Operators
#define   ADD                 10
#define   MINUS               11
#define   MULTIPLY            12
#define   DIVIDE              13
#define   MODULUS             14
#define   INC                 15
#define   DEC                 16

//Bitwise Operators
#define   BIT_NOT             20
#define   BIT_AND             21
#define   BIT_OR              22
#define   BIT_XOR             23
#define   L_SHIFT             24
#define   R_SHIFT             25

//Logical Operators
#define   LOGI_AND            30
#define   LOGI_OR             31
#define   NOT                 32

//Relational operators
#define   LESSER              40
#define   GREATER             41
#define   LESS_OR_EQUAL       42
#define   GREATER_OR_EQUAL    43
#define   EQUALITY            44
#define   NOT_EQUAL           45

//Assignment operators (shorthands)
#define   ASSIGNMENT          50
#define   SH_ADD              51
#define   SH_MINUS            52
#define   SH_MUL              53
#define   SH_DIV              54
#define   SH_MOD              55
#define   SH_BIT_AND          56
#define   SH_BIT_OR           57
#define   SH_BIT_XOR          58
#define   SH_L_SHIFT          59
#define   SH_R_SHIFT          60

//Misc. Operators
#define   EOL                 70
#define   OPEN_PARENT         71
#define   CLOSE_PARENT        72


#endif // SYMBOL_ID_H
