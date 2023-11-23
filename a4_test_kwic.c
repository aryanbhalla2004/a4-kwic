#include <stdio.h>
#include <string.h>

#include "a4_tests.h"
#include "a4_multimap.h"
#include "a4_kwic.h"


void test_typical_cases() {
  printf("\n*** Typical tests:\n\n");
  
  Multimap *mm;
  VERIFY_NOT_NULL(mm = mm_create(10));
  char stop_list[MAX_STOP_LIST][MAX_ENTRY_LENGTH];
  int stop_list_length = 0;
  // Testing stop file reading
  read_stop_list("nltk_stop.txt", stop_list, &stop_list_length);
  VERIFY_INT(1, is_in_stop_list("--", stop_list, &stop_list_length));
  VERIFY_INT(0, is_in_stop_list("Aran", stop_list, &stop_list_length));

  char paragraph_one[] = "Hey this is a paragraph test";
  char paragraph_two[] = "We are going to make a cookie.";
  process_paragraph(mm, 1, paragraph_one, 40, stop_list, &stop_list_length);
  process_paragraph(mm, 1, paragraph_one, 40, stop_list, &stop_list_length);
  process_paragraph(mm, 1, paragraph_one, 40, stop_list, &stop_list_length);
  process_paragraph(mm, 2, paragraph_two, 40, stop_list, &stop_list_length);
  process_paragraph(mm, 2, paragraph_two, 40, stop_list, &stop_list_length);
  
  printf("\n*** Before Post Processing :\n\n");
  print_KWIC_index(mm, "before.txt");
  post_process_index(mm, 2);
  printf("\n*** After Post Processing :\n\n");
  print_KWIC_index(mm, "output.txt");
}

void test_invalid() {
  printf("\n*** Invalid tests:\n\n");
  Multimap *mm;
  VERIFY_NOT_NULL(mm = mm_create(10));
  #if NDEBUG
    post_process_index(NULL, 4);
  #endif
}



int main() {
  
  test_typical_cases();
  test_invalid();

  if (0 == tests_failed()) {
    printf("\nAll %d tests passed.\n", tests_passed());
  } else {
    printf("\nFAILED %d of %d tests.\n", tests_failed(), tests_failed()+tests_passed());
  }
  
  printf("\n*** Tests complete.\n");  
  return 0;
}