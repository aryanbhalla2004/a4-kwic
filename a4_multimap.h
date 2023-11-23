#ifndef _A4_MULTIMAP_H
#define _A4_MULTIMAP_H
// These maximum string lengths include the \0.
// Your multimap should still work if these numbers change, though your
//  tests will be based on these numbers.
#define MAX_KEY_LENGTH 31
#define MAX_VALUE_LENGTH 81

typedef struct VALUE { int num; char str[MAX_VALUE_LENGTH]; } Value;
typedef struct MULTIMAP Multimap; // you need to define this yourself

typedef struct VALUE_NODE {
  Value value;
  struct VALUE_NODE *next;
} ValueNode;

typedef struct KEY_AND_VALUES {
  char key[MAX_KEY_LENGTH];
  int num_values;
  ValueNode *head;
} KeyAndValues;

struct MULTIMAP {
  int num_keys;
  int max_keys;
  int current_count;
  KeyAndValues *keys;
};

// Create a new multimap with at most "max_keys" keys.
// Return NULL on error.
Multimap *mm_create(int max_keys);

// Insert a new value into the multimap for the given key.
// If the key already exists in the multimap, the value is added to that key.
// Return the number of values associated with this key after insertion.
int mm_insert_value(Multimap *mm, char *key, int value_num, char *value_str);

// Count the number of keys in the multimap.
int mm_count_keys(Multimap *mm);

// Count the number of values associated with the given key.
// Zero means the key is not found in the multimap.
int mm_count_values(Multimap *mm, char *key);

// Copy as many values as possible for the given key into the array.
// Values will be ordered (and copied) numerically.
// Note that while values[] contains copies of the structs, the str pointers
//  still point to memory that belongs to the Multimap. Do not free or modify!
// Return the number of values copied, at most max_values.
int mm_get_values(Multimap *mm, char *key, Value values[], int max_values);

// Remove the key and all its corresponding values from the multimap.
// Return the number of values that were removed with the key.
// Zero means the key is not found in the multimap.
int mm_remove_key(Multimap *mm, char *key);

// Print the contents of the multimap, neatly.
// Keys and values must be in the correct order.
// This will be helpful for debugging and manual testing.
void mm_print(Multimap *mm);

// Destroy a multimap, freeing all allocated memory.
// Return the total number of keys AND values that were freed.
int mm_destroy(Multimap *mm);

// Using these functions together will allow you iterate over the keys in the
// multimap, in the order that they are stored (that is, sorted).

// They can be used as follows:

// char key[MAX_KEY_LENGTH];
// if (mm_get_first_key(mm, key, MAX_KEY_LENGTH) > 0) {
//   do {
//     // process the key
//   } while (mm_get_next_key(mm, key, MAX_KEY_LENGTH) > 0);
// }

// Consider what happens if mm_remove_key() is called (possibly more than
// once) as part of processing a key.

// Copy the first key string in the multimap into the array char *key.
// It will copy at most length-1 chars, always followed by \0.
// Returns -1 on error, 0 if there were no more keys, or 1 on success.
// If 0 or -1 are returned, then the contents of key are unchanged.
int mm_get_first_key(Multimap *mm, char *key, int length);

// Same as above, except it copies the next key following a call to either
// mm_get_first_key or mm_get_next_key.
int mm_get_next_key(Multimap *mm, char *key, int length);

#endif