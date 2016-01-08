#ifndef BKUTIL_H
#include <string.h>
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

#define BKUTIL_H
// Returns the Levenshtein Distance from 2 words.
int l_dist(char *first, char *second) {
  int firstLen = strlen(first);
  int secondLen = strlen(second);

  if (firstLen == 0) {
    return secondLen;
  }

  if (secondLen == 0) {
    return firstLen;
  }

  int dist[firstLen + 1][secondLen + 1];

  for (int i = 0; i <= firstLen; i++) {
    dist[i][0] = i;
  }

  for (int i = 0; i <= secondLen; i++) {
    dist[0][i] = i;
  }

  for (int i = 1; i <= firstLen; i++) {
    for (int j = 1; j <= secondLen; j++) {
      int match = first[i - 1] == second[j - 1] ? 0 : 1;

      dist[i][j] = min(min(dist[i][j - 1] + 1, dist[i - 1][j] + 1), dist[i - 1][j - 1] + match);
    }
  }

  int res = dist[firstLen][secondLen];

  return res;
};

#endif
