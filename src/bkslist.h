#ifndef SEARCH_LIST_H
#define SEARCH_LIST_H

#include <inttypes.h>

typedef struct SearchList SearchList;
struct SearchList {
  uint8_t** list;
  uint64_t size;
  uint64_t used;
};

SearchList init_search_list();
void list_add(void *word_arg, SearchList* s_list);
extern void free_list(uint8_t** list);

#endif
