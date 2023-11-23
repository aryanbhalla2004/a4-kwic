#include <stdio.h>
#include <string.h>

#include "a4_multimap.h"
#include "a4_tests.h"

void test_unusual_cases() {
  printf("\n*** Test Unusual tests:\n\n");
  
  #ifdef NDEBUG
    Multimap *mm;
   
    VERIFY_NOT_NULL(mm = mm_create(5));
    VERIFY_INT(1, mm_insert_value(mm, "hello", 123, "abc"));
    
    VERIFY_INT(-1, mm_get_first_key(mm, "hello", 0));
    VERIFY_INT(-1, mm_get_first_key(NULL, "hello", 0));
    VERIFY_INT(-1, mm_get_first_key(mm, NULL, 0));
    VERIFY_INT(-1, mm_get_first_key(NULL, NULL, -5));
    VERIFY_INT(-1, mm_get_first_key(mm, "?", -5));
    VERIFY_INT(-1, mm_get_first_key(mm, "asdasdasdasdasd", 5));
    VERIFY_INT(-1, mm_get_first_key(mm, "123123", 50));

    mm_destroy(mm);
  #endif
  
}

void test_typical_cases() {
  printf("\n*** Typical tests:\n\n");
  Multimap *mm;
  char key[MAX_KEY_LENGTH] = "";
  char last[MAX_KEY_LENGTH] = "";

  VERIFY_NOT_NULL(mm = mm_create(5));
  VERIFY_INT(0, mm_get_first_key(mm, "Hello", MAX_KEY_LENGTH));
  VERIFY_INT(1, mm_insert_value(mm, "hello", 123, "abc"));
  VERIFY_INT(1, mm_get_first_key(mm, key, MAX_KEY_LENGTH));
  VERIFY_STR("hello", key);
  VERIFY_INT(0, mm_get_next_key(mm, last, MAX_KEY_LENGTH));

  VERIFY_INT(1, mm_insert_value(mm, "hello_last", 1213, "asdasd"));
  VERIFY_INT(1, mm_get_next_key(mm, last, MAX_KEY_LENGTH));
  VERIFY_STR("hello_last", last);

  VERIFY_INT(1, mm_remove_key(mm, "hello"));
  VERIFY_INT(1, mm_get_first_key(mm, key, MAX_KEY_LENGTH));
  VERIFY_STR("hello_last", key);
  VERIFY_INT(0, mm_get_next_key(mm, last, MAX_KEY_LENGTH));

  mm_destroy(mm);
}

void test_invalid() {
  printf("\n*** Invalid tests:\n\n");
  #ifdef NDEBUG
    Multimap *mm;
    char key_value[MAX_KEY_LENGTH] = "";

    VERIFY_NOT_NULL(mm = mm_create(4));
    VERIFY_INT(0, mm_get_first_key(mm, key_value, MAX_KEY_LENGTH));
    VERIFY_STR("", key_value);

    mm_destroy(mm);
  #endif
}

void test_bounds() {
  printf("\n*** Testing Bounds:\n\n");

  Multimap *mm;
  char key[MAX_KEY_LENGTH] = "";
  char last_key[MAX_KEY_LENGTH] = "";

  VERIFY_NOT_NULL(mm = mm_create(5));
  VERIFY_INT(1, mm_insert_value(mm, "hello", 123, "abc"));
  VERIFY_INT(1, mm_get_first_key(mm, key, MAX_KEY_LENGTH));
  VERIFY_STR("hello", key);
  VERIFY_INT(1, mm_insert_value(mm, "hello_last", 1213, "asdasd"));
  VERIFY_INT(1, mm_get_next_key(mm, last_key, MAX_KEY_LENGTH));
  VERIFY_STR("hello_last", last_key);
  
  mm_destroy(mm);
}


int main() {
  
  test_bounds();
  test_typical_cases();
  test_invalid();
  test_unusual_cases();



  if (0 == tests_failed()) {
    printf("\nAll %d tests passed.\n", tests_passed());
  } else {
    printf("\nFAILED %d of %d tests.\n", tests_failed(), tests_failed()+tests_passed());
  }
  
  printf("\n*** Tests complete.\n");  
  return 0;
}
