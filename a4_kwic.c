#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "a4_kwic.h"
#include "a4_multimap.h"


void read_stop_list(char *filename, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length) {
  assert(filename != NULL && stop_list != NULL);

  FILE *file = fopen(filename, "r");
  char word[MAX_ENTRY_LENGTH];

  if (file != NULL) { 
    while (fgets(word, sizeof(word), file) != NULL && *stop_list_length < MAX_STOP_LIST) {
      word[strlen(word) - 1] = '\0';
      strncpy(stop_list[*stop_list_length], word, MAX_ENTRY_LENGTH - 1);
      stop_list[*stop_list_length][MAX_ENTRY_LENGTH - 1] = '\0';
      (*stop_list_length)++;
    }
  }

  fclose(file);
}

int is_in_stop_list(char *word, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length) {
  assert(word != NULL);
  
  if(*stop_list_length > 0 && stop_list != NULL && word != NULL) {
    for(int i = 0; i < *stop_list_length; i++) {
      if(strcasecmp(stop_list[i], word) == 0) {
        return 1;
      }
    }
  }
  

  return 0;
}


void process_paragraph(Multimap *mm, int paragraph_number, char *paragraph, int entry_length, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length) {
  assert(mm != NULL);
  assert(paragraph_number > 0 && entry_length > 0);
  assert(paragraph != NULL && stop_list != NULL);

  int paragraph_length = strlen(paragraph);
  char *word;
  const char delimiters[] = " \n";

  char temp_paragraph[paragraph_length];
  temp_paragraph[0] = '\0';
  strcpy(temp_paragraph, paragraph);

  word = strtok(paragraph, delimiters);

  int start = -1;
  while (word != NULL) {
    int word_length = strlen(word);
    int end = (start + word_length) + 1;
    if(is_in_stop_list(word, stop_list, stop_list_length) == 0 && strlen(word) > 0) {
      int context = (entry_length - word_length) / 2;
      char first_sentence[context];
      char last_sentence[context];
      first_sentence[0] = '\0';
      last_sentence[0] = '\0';
      
      if(start > 0) {
        int index = 0;
        int start_point = start < context ? 0 : (start - context) + 1;

        while(start_point != 0 && temp_paragraph[start_point - 1] != ' ') {
          start_point++;
        }
        
        for(int i = start_point; i <= start; i++) {
          first_sentence[index++] = temp_paragraph[i];
        }

        first_sentence[index] = '\0';
      }

      if(end <= paragraph_length) {
        int index = 0;
        int total_length = end + context;
        int end_point = total_length > paragraph_length ? paragraph_length : total_length;

        while(temp_paragraph[end_point + 1] != ' ') {
          end_point--;
        }

        for(int i = end; i <= end_point; i++) {
          last_sentence[index++] = temp_paragraph[i];
        }

        last_sentence[index] = '\0';
      }

      char context_string[(context * 2) + word_length];
      context_string[0] = '\0';
      strcat(context_string, first_sentence);
      strcat(context_string, word);
      strcat(context_string, last_sentence);
    
      mm_insert_value(mm, word, paragraph_number, context_string);
    }

    start += word_length + 1;
    word = strtok(NULL, delimiters);
  }
}

void post_process_index(Multimap *mm, int max_frequency) {
  assert(mm != NULL && max_frequency >= 0);

  if(mm != NULL && max_frequency >= 0) {
    char key[MAX_KEY_LENGTH];
    if (mm_get_first_key(mm, key, MAX_KEY_LENGTH) > 0) {
      do {
        int values = mm_count_values(mm, key);
        if(values > max_frequency) {
          mm_remove_key(mm, key);
        }
      } while (mm_get_next_key(mm, key, MAX_KEY_LENGTH) > 0);
    }
  }
  
}

void print_KWIC_index(Multimap *mm, char *out_file) {
  assert(mm != NULL);

  if(mm != NULL) {
    char key[MAX_KEY_LENGTH];
    int num_values;
    Value values[MAX_ENTRY_LENGTH];
    FILE *out = fopen(out_file, "w");

    printf("%-20s %-4s %s\n", "WORD", "PARA", "CONTEXT");
    printf("%-20s %-4s %s\n", "====", "====", "=======");

    if(NULL != out) {
      fprintf(out, "%-20s %-4s %s\n", "WORD", "PARA", "CONTEXT");
      fprintf(out, "%-20s %-4s %s\n", "====", "====", "=======");
    }

    if (mm_get_first_key(mm, key, MAX_KEY_LENGTH) > 0) {
      do {
        num_values = mm_get_values(mm, key, values, MAX_ENTRY_LENGTH);
        for(int i = 0; i < num_values; i++) {
          printf("%-23s %d %s\n", key, values[i].num, values[i].str);

          if(NULL != out) {
            fprintf(out, "%-23s %d %s\n", key, values[i].num, values[i].str);
          }
        }
      } while (mm_get_next_key(mm, key, MAX_KEY_LENGTH) > 0);
    }

    fclose(out);
  }
}

void read_input_file(Multimap *mm, char *in_file, int entry_length, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length) {
  assert(entry_length > 0);
  assert(in_file != NULL);
  assert(mm != NULL);

  if(in_file != NULL || mm != NULL) {
    FILE *file = fopen(in_file, "r");

    if (file != NULL) { 
      int paragraph_number = 1;
      char line[MAX_PARAGRAPH];
      char paragraph[MAX_PARAGRAPH];
    
      while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "\n") == 0) {
          process_paragraph(mm, paragraph_number, paragraph, entry_length, stop_list, stop_list_length);
          paragraph_number++;
          paragraph[0] = '\0';
        } else {
          char delimiters[] = " \n";
          char *word = strtok(line, delimiters);
          while(word != NULL) {
            int word_length = strlen(word);
            char clean_word[word_length + 1];
            int current_index = 0;
            for(int i = 0; i < word_length; i++) {
              if(isalnum(word[i]) || word[i] == '-' || word[i] == '\'') {
                clean_word[current_index++] = word[i];
              }
            }

            clean_word[current_index] = ' ';
            clean_word[current_index + 1] = '\0';
            strcat(paragraph, clean_word); 
            word = strtok(NULL, delimiters);
          }

          if(feof(file) > 0) {
            process_paragraph(mm, paragraph_number, paragraph, entry_length, stop_list, stop_list_length);
            paragraph_number++;
            paragraph[0] = '\0';
          }
        }
      }
    }

    fclose(file);
  }
}

void kwic(char* in_file, char* out_file, char* stop_file, int max_frequency, int entry_length, int max_index) {
  assert(in_file != NULL);
  assert(max_frequency >= 0 && entry_length > 0 && max_index > 0);
  
  Multimap *mm = mm_create(max_index);

  char stop_list[MAX_STOP_LIST][MAX_ENTRY_LENGTH];
  int stop_list_length = 0;

  if(stop_file != NULL) {
    read_stop_list(stop_file, stop_list, &stop_list_length);
  }

  read_input_file(mm, in_file, entry_length, stop_list, &stop_list_length);
  post_process_index(mm, max_frequency);
  print_KWIC_index(mm, out_file);
  mm_destroy(mm);
}