#include <stdio.h>
#include <string.h>
/*** Example tests (should pass as-is) ***/
#include "a4_tests.h"
#include "a4_multimap.h"

void test_example() {
  Multimap *mm;
  Value arr[3]; // using one extra space
  
  printf("\n*** Example tests:\n\n");
  
  VERIFY_NOT_NULL(mm = mm_create(3));
  VERIFY_INT(1, mm_insert_value(mm, "hello", 123, "abc"));
  VERIFY_INT(1, mm_insert_value(mm, "world", 456, "def"));
  VERIFY_INT(2, mm_insert_value(mm, "hello", 999, "zzz"));
  // maximum value length:
  VERIFY_INT(3, mm_insert_value(mm, "hello", 1048576, "abcdefghijabcdefghij"));
  // there is no connection between this value and the one with the key "hello"
  VERIFY_INT(1, mm_insert_value(mm, "!", 123, "abc"));
  VERIFY_INT(2, mm_insert_value(mm, "!", 5, ""));
  
  VERIFY_INT(3, mm_count_keys(mm));
  VERIFY_INT(3, mm_count_values(mm, "hello"));
  VERIFY_INT(1, mm_count_values(mm, "world"));
  VERIFY_INT(2, mm_count_values(mm, "!"));
  VERIFY_INT(0, mm_count_values(mm, "?"));
  
  VERIFY_INT(2, mm_get_values(mm, "!", arr, 2));
  VERIFY_INT(5, arr[0].num);
  VERIFY_STR("", arr[0].str);
  VERIFY_INT(123, arr[1].num);
  VERIFY_STR("abc", arr[1].str);
  
  // no space for more keys
  VERIFY_INT(-1, mm_insert_value(mm, "xyz", 789, "ghi"));

  VERIFY_INT(2, mm_remove_key(mm, "!"));
  VERIFY_INT(0, mm_count_values(mm, "!"));
  VERIFY_INT(1, mm_insert_value(mm, "xyz", 789, "ghi"));

  // make sure we aren't copying three values when we ask for two
  arr[2].num = 0;
  arr[2].str[0] = '\0';
  VERIFY_INT(2, mm_get_values(mm, "hello", arr, 2));
  VERIFY_INT(123, arr[0].num);
  VERIFY_STR("abc", arr[0].str);
  VERIFY_INT(999, arr[1].num);
  VERIFY_STR("zzz", arr[1].str);
  VERIFY_INT(0, arr[2].num);
  VERIFY_STR("", arr[2].str);
  
  // this isn't a test:
  mm_print(mm);
  
  VERIFY_INT(8, mm_destroy(mm));
  
  // these could fail assertions but should execute and pass with assertions off
#ifdef NDEBUG
  VERIFY_NULL(mm_create(-1));
  VERIFY_INT(-1, mm_insert_value(NULL, "hello", 123, "abc"));
#endif
}

/*** Your test code and main program follow ***/

void test_typical() {
  Multimap *mm;
  Value arr[5];
  char key[] = "hello";
  char value[] = "goodbye";
  
  printf("\n*** Typical tests:\n\n");
  
  VERIFY_NOT_NULL(mm = mm_create(7));
  VERIFY_INT(1, mm_insert_value(mm, key, 99, value));
  key[0] = 'y';
  value[0] = 'f';
  VERIFY_INT(1, mm_insert_value(mm, key, 0, value));
  VERIFY_INT(1, mm_insert_value(mm, "aaa", 555, ""));
  VERIFY_INT(1, mm_insert_value(mm, "III", -555, ""));
  VERIFY_INT(2, mm_insert_value(mm, "hello", -1, "iii"));
  VERIFY_INT(2, mm_insert_value(mm, "yello", 1234567, value));
  VERIFY_INT(3, mm_insert_value(mm, "Hello", 123456789, "GOODBYE"));
  VERIFY_INT(4, mm_insert_value(mm, "hELLO", -1, "Goodbye"));
  VERIFY_INT(5, mm_insert_value(mm, "HELLO", -1, "zzzzalphaend"));
  VERIFY_INT(1, mm_insert_value(mm, "Hellow", 0, ""));
  VERIFY_INT(1, mm_insert_value(mm, "H", 0, ""));
  VERIFY_INT(2, mm_insert_value(mm, "h", 1, ""));
  VERIFY_INT(1, mm_insert_value(mm, "???", 0, ""));
  VERIFY_INT(3, mm_insert_value(mm, "h", -1, ""));
  VERIFY_INT(4, mm_insert_value(mm, "h", 0, ""));

  VERIFY_INT(7, mm_count_keys(mm));
  
  printf("Inspect key ordering:\n");
  mm_print(mm);
  
  VERIFY_INT(1, mm_count_values(mm, "???"));
  VERIFY_INT(0, mm_count_values(mm, "??"));
  VERIFY_INT(0, mm_count_values(mm, "????"));
  VERIFY_INT(1, mm_count_values(mm, "aaa"));
  VERIFY_INT(4, mm_count_values(mm, "h"));
  VERIFY_INT(5, mm_count_values(mm, "hello"));
  VERIFY_INT(5, mm_count_values(mm, "Hello"));
  VERIFY_INT(5, mm_count_values(mm, "HELLO"));
  VERIFY_INT(5, mm_count_values(mm, "hELLO"));
  VERIFY_INT(1, mm_count_values(mm, "hellow"));
  VERIFY_INT(0, mm_count_values(mm, "hell"));
  VERIFY_INT(1, mm_count_values(mm, "iii"));
  VERIFY_INT(2, mm_count_values(mm, "yello"));
 
  VERIFY_INT(5, mm_get_values(mm, "HELLO", arr, 5));
  VERIFY_INT(-1, arr[0].num);
  VERIFY_STR("zzzzalphaend", arr[0].str);
  VERIFY_INT(-1, arr[1].num);
  VERIFY_STR("Goodbye", arr[1].str);
  VERIFY_INT(-1, arr[2].num);
  VERIFY_STR("iii", arr[2].str);
  VERIFY_INT(99, arr[3].num);
  VERIFY_STR("goodbye", arr[3].str);
  VERIFY_INT(123456789, arr[4].num);
  VERIFY_STR("GOODBYE", arr[4].str);

  VERIFY_INT(2, mm_get_values(mm, "yello", arr, 5));
  VERIFY_INT(0, arr[0].num);
  VERIFY_STR("foodbye", arr[0].str);
  VERIFY_INT(1234567, arr[1].num);
  VERIFY_STR("foodbye", arr[1].str);

  VERIFY_INT(2, mm_remove_key(mm, key));
  VERIFY_INT(0, mm_count_values(mm, "yello"));
  VERIFY_INT(0, mm_get_values(mm, "yello", arr, 5));
  VERIFY_INT(5, mm_count_values(mm, "hello"));
  VERIFY_INT(6, mm_count_keys(mm));

  VERIFY_INT(5, mm_remove_key(mm, "hello"));
  VERIFY_INT(0, mm_count_values(mm, "hello"));
  
  VERIFY_INT(1, mm_insert_value(mm, "hello", -123, ""));
  VERIFY_INT(1, mm_get_values(mm, "hello", arr, 5));
  VERIFY_INT(-123, arr[0].num);
  VERIFY_STR("", arr[0].str);

  VERIFY_INT(3, mm_get_values(mm, "h", arr, 3));
  VERIFY_INT(-1, arr[0].num);
  VERIFY_STR("", arr[0].str);
  VERIFY_INT(0, arr[1].num);
  VERIFY_STR("", arr[1].str);
  VERIFY_INT(0, arr[2].num);
  VERIFY_STR("", arr[2].str);
  
  VERIFY_INT(15, mm_destroy(mm));
}

void test_empty()
{
  Multimap *mm;
  Value arr[1];
  
  printf("\n*** Empty multimap:\n\n");
  
  VERIFY_NOT_NULL(mm = mm_create(1));
  VERIFY_INT(0, mm_count_keys(mm));
  VERIFY_INT(0, mm_count_values(mm, "abc"));
  VERIFY_INT(0, mm_remove_key(mm, "A"));
  VERIFY_INT(0, mm_get_values(mm, "", arr, 1));
  printf("Nothing printed here:\n");
  mm_print(mm);
  VERIFY_INT(0, mm_destroy(mm));
}

void test_edge()
{
  Multimap *mm;
  Value arr[3];

  printf("\n*** Edge cases:\n\n");
  
  VERIFY_NOT_NULL(mm = mm_create(0));
  VERIFY_INT(-1, mm_insert_value(mm, "abc", 123, "def"));
  VERIFY_INT(0, mm_count_keys(mm));
  VERIFY_INT(0, mm_count_values(mm, "abc"));
  VERIFY_INT(0, mm_remove_key(mm, "A"));
  VERIFY_INT(0, mm_get_values(mm, "", arr, 1));
  printf("Nothing printed here:\n");
  mm_print(mm);
  VERIFY_INT(0, mm_destroy(mm));

  VERIFY_NOT_NULL(mm = mm_create(5));
  VERIFY_INT(1, mm_insert_value(mm, "cccccccccc", 3, "cccccccccccccccccccc"));
  VERIFY_INT(1, mm_insert_value(mm, "bbbbbbbbbbb", 2, "bbbbbbbbbbbbbbbbbbbbb"));
  VERIFY_INT(1, mm_insert_value(mm, "aaaaaaaaaa", 999, "aaa"));
  //VERIFY_INT(2, mm_insert_value(mm, "aaaaaaaaaaaaaaa", 1, "aaaaaaaaaaaaaaaaaaaaa"));
  VERIFY_INT(1, mm_insert_value(mm, "dddddddddd", 4, "ddd"));
  //VERIFY_INT(1, mm_insert_value(mm, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee", 5, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"));
  //VERIFY_INT(2, mm_insert_value(mm, "eeeeeeeeee", -999, "ddd"));
  //VERIFY_INT(-1, mm_insert_value(mm, "", 0, "")); // full

  printf("Inspect key ordering:\n");
  mm_print(mm);
  
  VERIFY_INT(0, mm_count_values(mm, "aaaaaaaaa"));
  //VERIFY_INT(1, mm_count_values(mm, "bbbbbbbbbbbbb"));
  //VERIFY_INT(1, mm_count_values(mm, "ccccccccccc"));
  //VERIFY_INT(1, mm_count_values(mm, "ddddddddddddddddd"));
  //VERIFY_INT(2, mm_count_values(mm, "eeeeeeeeeeeeeeeeeeeeeee"));

  //VERIFY_INT(2, mm_get_values(mm, "aaaaaaaaaaa", arr, 3));
  //VERIFY_INT(1, arr[0].num);
  //VERIFY_STR("aaaaaaaaaaaaaaaaaaaa", arr[0].str);
  //VERIFY_INT(999, arr[1].num);
  //VERIFY_STR("aaa", arr[1].str);
  VERIFY_INT(0, mm_get_values(mm, "eeeeeeeee", arr, 3));
  //VERIFY_INT(2, mm_get_values(mm, "eeeeeeeeeeeee", arr, 3));
  //VERIFY_INT(-999, arr[0].num);
  //VERIFY_STR("ddd", arr[0].str);
  //VERIFY_INT(5, arr[1].num);
  //VERIFY_STR("eeeeeeeeeeeeeeeeeeee", arr[1].str);

  VERIFY_INT(0, mm_get_values(mm, "aaaaaaaaaaa", arr, 0));

  //VERIFY_INT(2, mm_remove_key(mm, "eeeeeeeeeeeeeeeeee"));
  //VERIFY_INT(1, mm_insert_value(mm, "", 0, "")); // not full
  
  //VERIFY_INT(5, mm_count_keys(mm));
  //VERIFY_INT(1, mm_count_values(mm, ""));

  //VERIFY_INT(1, mm_remove_key(mm, ""));
  
  //VERIFY_INT(9, mm_destroy(mm));
}

void test_multiple()
{
  Multimap *mm1, *mm2, *mm3;
  Value arr[3];

  printf("\n*** More than one independent multimap:\n\n");
  
  VERIFY_NOT_NULL(mm1 = mm_create(1));
  VERIFY_NOT_NULL(mm2 = mm_create(2));
  VERIFY_NOT_NULL(mm3 = mm_create(3));
  
  VERIFY_INT(1, mm_insert_value(mm1, "abc", -1, "def"));
  VERIFY_INT(2, mm_insert_value(mm1, "abc", 1, "ghi"));
  VERIFY_INT(1, mm_insert_value(mm3, "abc", 10, "mno"));
  VERIFY_INT(1, mm_insert_value(mm3, "pqr", 100, "stu"));
  
  VERIFY_INT(1, mm_count_keys(mm1));
  VERIFY_INT(0, mm_count_keys(mm2));
  VERIFY_INT(2, mm_count_keys(mm3));

  VERIFY_INT(2, mm_count_values(mm1, "abc"));
  VERIFY_INT(0, mm_count_values(mm1, "jkl"));
  VERIFY_INT(0, mm_count_values(mm2, "abc"));
  VERIFY_INT(1, mm_count_values(mm3, "abc"));
  VERIFY_INT(1, mm_count_values(mm3, "pqr"));
  
  VERIFY_INT(2, mm_get_values(mm1, "abc", arr, 3));
  VERIFY_STR("def", arr[0].str);
  VERIFY_STR("ghi", arr[1].str);
  VERIFY_INT(0, mm_get_values(mm2, "abc", arr, 3));
  VERIFY_INT(1, mm_get_values(mm3, "abc", arr, 3));
  VERIFY_STR("mno", arr[0].str);
  
  VERIFY_INT(1, mm_remove_key(mm3, "abc"));
  VERIFY_INT(2, mm_count_values(mm1, "abc"));
  
  VERIFY_INT(3, mm_destroy(mm1));

  VERIFY_INT(0, mm_count_values(mm2, "abc"));
  VERIFY_INT(0, mm_count_values(mm3, "abc"));
  VERIFY_INT(1, mm_count_values(mm3, "pqr"));

  VERIFY_INT(0, mm_destroy(mm2));
  VERIFY_INT(2, mm_destroy(mm3));
}

void test_invalid()
{
  Multimap *mm;
  Value arr[2];

  printf("\n*** Invalid cases:\n\n");

  // we need a valid multimap for some of these
  VERIFY_NOT_NULL(mm = mm_create(1));
  VERIFY_INT(1, mm_insert_value(mm, "hello", 123, "abc"));
  
  VERIFY_INT(-1, mm_insert_value(mm, NULL, 123, "abc"));
  VERIFY_INT(-1, mm_insert_value(mm, "hello", 123, NULL));
 
  VERIFY_INT(-1, mm_count_keys(NULL));
  VERIFY_INT(-1, mm_count_values(NULL, "hello"));
  VERIFY_INT(-1, mm_count_values(mm, NULL));
  
  VERIFY_INT(-1, mm_get_values(NULL, "hello", arr, 2));
  VERIFY_INT(-1, mm_get_values(mm, NULL, arr, 2));
  VERIFY_INT(-1, mm_get_values(mm, "hello", NULL, 2));
  VERIFY_INT(-1, mm_get_values(mm, "hello", arr, -1));

  VERIFY_INT(-1, mm_remove_key(NULL, "hello"));
  VERIFY_INT(-1, mm_remove_key(mm, NULL));

  VERIFY_INT(-1, mm_destroy(NULL));

  // make sure none of this put us in a bad state
  VERIFY_INT(1, mm_count_keys(mm));
  VERIFY_INT(1, mm_count_values(mm, "hello"));
  VERIFY_INT(2, mm_destroy(mm));
}

int main() {
  printf("*** Starting tests...\n");
  
  // Leave this:
  test_example();
  
  /*** Call your own tests here ***/

  test_typical();
  test_empty();
  test_edge();
  test_multiple();
#ifdef NDEBUG
  test_invalid();
#endif
  
  // And this:
  if (0 == tests_failed()) {
    printf("\nAll %d tests passed.\n", tests_passed());
  } else {
    printf("\nFAILED %d of %d tests.\n", tests_failed(), tests_failed()+tests_passed());
  }
  
  printf("\n*** Tests complete.\n");  
  return 0;
}