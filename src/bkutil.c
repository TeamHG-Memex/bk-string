#include "bkutil.h"

// Returns the Levenshtein Distance from 2 words.
uint64_t l_dist(void *first_word, void *second_word) {
  uint8_t *first = first_word;
  uint8_t *second = second_word;
  uint64_t firstLen = strlen(first);
  uint64_t secondLen = strlen(second);

  if (firstLen == 0) {
    return secondLen;
  }

  if (secondLen == 0) {
    return firstLen;
  }

  int dist[firstLen + 1][secondLen + 1];

  for (uint64_t i = 0; i <= firstLen; i++) {
    dist[i][0] = i;
  }

  for (uint64_t i = 0; i <= secondLen; i++) {
    dist[0][i] = i;
  }

  for (uint64_t i = 1; i <= firstLen; i++) {
    for (uint64_t j = 1; j <= secondLen; j++) {
      uint64_t match = first[i - 1] == second[j - 1] ? 0 : 1;

      dist[i][j] = min(min(dist[i][j - 1] + 1, dist[i - 1][j] + 1), dist[i - 1][j - 1] + match);
    }
  }

  uint64_t res = dist[firstLen][secondLen];

  return res;
};

// Returns the allocated memory, or exits with an error.
void *handle_calloc(uint64_t len, size_t size) {
  void *alloc = calloc(len, size);
  if (!alloc) {
    fprintf(stderr, "out of memory\n");
    exit(EXIT_FAILURE);
  }

  return alloc;
}
