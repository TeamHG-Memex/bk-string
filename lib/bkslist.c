#ifndef SEARCH_LIST_H
#define SEARCH_LIST_H
#include <stdlib.h>

const int LIST_LEN = 2;

typedef struct SearchList SearchList;
struct SearchList {
  char** list;
  int size;
  int used;
};

// Doubles the size of a given Search List
void grow_list(SearchList *s_list) {
  int newSize = s_list->size * 2 * sizeof(char*);
  char** temp = malloc(newSize);

  // Add all items in the Search List to the temp array.
  for (int i = 0; i <= s_list->size; i++) {
    temp[i] = s_list->list[i];
  }

  // Allocate the Search List
  s_list->list = malloc(newSize);
  s_list->list = temp;
  free(temp);

  // Set the Search List's size.
  s_list->size = s_list->size * 2;
};

// Adds a word to a Search List
void list_add(char *word, SearchList* s_list) {
  // If the number of indexes used is equal to the size of the array:
  if (s_list->used == s_list->size) {
    // Make the array larger.
    grow_list(s_list);
  }

  // Add the word to the last index in the array.
  s_list->list[s_list->used] = word;
  s_list->used += 1;
};

// Returns an initialized Search List
SearchList init_search_list() {
  SearchList s_list;
  s_list.list = malloc(sizeof(char*) * LIST_LEN);
  s_list.size = LIST_LEN;
  s_list.used = 0;

  return s_list;
};
#endif
