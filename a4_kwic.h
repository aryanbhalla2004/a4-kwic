#ifndef A4_KWIC_H

#define A4_KWIC_H
#include "a4_multimap.h"

#define MAX_ENTRY_LENGTH 100 // Adjust as needed
#define MAX_STOP_LIST 1000 // Adjust as needed
#define MAX_PARAGRAPH 20000 // Adjust as needed

void read_stop_list(char *filename, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length);
int is_in_stop_list(char *word, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length);
void process_paragraph(Multimap *mm, int paragraph_number, char *paragraph, int entry_length, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length);
void post_process_index(Multimap *mm, int max_frequency);
void print_KWIC_index(Multimap *mm, char *out_file);
void read_input_file(Multimap *mm, char *in_file, int entry_length, char stop_list[][MAX_ENTRY_LENGTH], int *stop_list_length);
void kwic(char* in_file, char* out_file, char* stop_file, int max_frequency, int entry_length, int max_index);

#endif