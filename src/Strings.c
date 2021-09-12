#include "Strings.h"

//returns string equivalent of an integer
char *convertIntToStr(int num) {
  int len = snprintf(NULL, 0, "%d", num);
  char *resultStr = malloc(len + 1);
  snprintf(resultStr, len + 1, "%d", num);
  return resultStr;
}

//concatenate two strings
char *concat(char *s1, char *s2) {
  char *result = realloc(s1, strlen(s1) + strlen(s2) + 1);
  strcat(result, s2);
  free(s2);
  return result;
}

//return a malloc'ed string
char *createString(char *str) {
  char *newStr;
  int len;
  if(str) {
    len = strlen(str);
    newStr = malloc(len+1);
    while(newStr == NULL)
      newStr = malloc(len+1);
    strncpy(newStr, str, len);
    newStr[len] = '\0';
    return newStr;
  }else
    return NULL;
}