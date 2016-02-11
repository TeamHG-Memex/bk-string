#ifndef BKUTIL_H
#define BKUTIL_H

#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Returns the maximum of 2 given values.
#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; \
  })

// Returns the minimum of 2 given values.
#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; \
  })


uint64_t l_dist(void *first_word, void *second_word);
uint8_t char_cmp(uint8_t *char1, uint8_t *char2);
uint8_t is_not_continued_byte(uint8_t byte);
void *handle_calloc(uint64_t len, size_t size);

#endif
