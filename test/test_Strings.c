#ifdef TEST

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Strings.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_convertIntToStr_given_integer_expect_integer_str_is_returned(void) {
  int num = 48;
  
  char *str = convertIntToStr(num);
  TEST_ASSERT_EQUAL_STRING("48", str);
  
  free(str);
}

void test_createString_given_a_str_expect_malloced_string_is_returned() {
  char *str = "Hello World\n";
  
  char *returnedStr = createString(str);
  TEST_ASSERT_EQUAL_STRING("Hello World\n", returnedStr);
  
  free(returnedStr);
}

void test_concat_given_two_strings_expect_strings_concatenated_after_calling_concat() {
  char *str1 = createString("Bye World ");
  char *str2 = createString("Hello World\n");
  
  char *resultStr = concat(str1, str2);
  TEST_ASSERT_EQUAL_STRING("Bye World Hello World\n", resultStr);
  
  free(resultStr);
}

#endif // TEST
