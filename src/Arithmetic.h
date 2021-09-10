#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#define infixCalculation(operand1, operand2, operator)                            \
  int result = (operand1)operator(operand2);
  
#define prefixCalculation(operand, operator)                                      \
  int result = operator operand;
  
#define suffixCalculation(operand, operator)                                      \
  int result = operand operator;

#define createInfixFunction(funcName, operator)                                   \
int funcName(int number1, int number2) {                                          \
  infixCalculation(number1, number2, operator);                                   \
  return result;                                                                  \
}

#define createPrefixFunction(funcName, operator)                                  \
int funcName(int number, int dummy) {                                           \
  prefixCalculation(number, operator);                                            \
  return result;                                                                  \
}

#define createSuffixFunction(funcName, operator)                                  \
int funcName(int number, int dummy) {                                           \
  suffixCalculation(number, operator);                                            \
  return result;                                                                  \
}

#endif // ARITHMETIC_H
