#include "bkutil.h"
#include "bkslist.h"

const uint64_t LIST_LEN = 1024;

// Doubles the size of a given Search List
void grow_list(SearchList *s_list) {
  // Add the word to the last index in the array.
  uint64_t newSize = s_list->size * 2;
  uint8_t** temp = handle_calloc(newSize, sizeof(uint8_t*));

  // Add all items in the Search List to the temp array.
  for (uint64_t i = 0; i < s_list->size; i++) {
    temp[i] = s_list->list[i];
  }

  free(s_list->list);

  // Allocate the Search List
  s_list->list = temp;

  // Set the new Search List size.
  s_list->size = newSize;
};

// Adds a word to a Search List
void list_add(void *word_arg, SearchList* s_list) {
  uint8_t *word = word_arg;
  // If the number of indexes used is equal to the size of the array:
  if (s_list->used == s_list->size) {
    // Make the array larger.
    grow_list(s_list);
  }

  s_list->list[s_list->used] = word;
  s_list->used += 1;
};

// Returns an initialized Search List
SearchList init_search_list() {
  SearchList s_list;
  s_list.size = LIST_LEN;
  s_list.used = 0;
  s_list.list = handle_calloc(LIST_LEN, sizeof(uint8_t*));

  return s_list;
};
