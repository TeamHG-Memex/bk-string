#include "bkutil.h"
#include "character.h"

// TODO:0 Implement Jaccard Distance
// TODO:40 Handle Levenshtein Distance with n-grams
// TODO:30 Handle Jaccard Distance with n-grams
// TODO:20 Handle modified Jaccard Distance with n-grams

// Returns a modified Jaccard Distance.  Distance is returned between 0 and 100, where 0 would be complete
// overlap between 2 strings, and 100 would be no overlap.
//
// Intersect and Unions in datasets consider how many of each character matches.
//
// Example:
// "GG" has 2 intersection characters with "GGGG" and a union of 4.
uint64_t mod_j_dist(void *first_word, void *second_word) {
  // Check to make sure the words exist before attempting to use distance on them.
  if (first_word == NULL || second_word == NULL) {
    return 100;
  }

  // Map to UTF-8 characters
  Character *first = map_chr_str(first_word);
  Character *second = map_chr_str(second_word);
  uint64_t len1 = chr_strlen(first);
  uint64_t len2 = chr_strlen(second);

  if (len1 == 0 || len2 == 0) {
    return 100;
  }

  uint64_t intersect_count = 0;
  uint64_t union_count = len1 + len2;

  Character null = map_chr(NULL);

  for (uint64_t i = 0; i < len1; i++) {
    for (uint64_t j = 0; j < len2; j++) {
      // If the second word's character is not NULL and equal to the first word's character:
      if (chrcmp(second[j], null) && !chrcmp(first[i], second[j])) {
        // Increment intersect, decrement union, and set the current character for the second word to NULL.
        intersect_count += 1;
        union_count -= 1;
        second[j] = null;
        break;
      }
    }
  }

  free_chr_str(first);
  free_chr_str(second);
  free_chr(&null);

  uint64_t dist = 100 - ((100 * intersect_count) / union_count);

  return dist;
}

// Returns the Levenshtein Distance from 2 strings.  The strings are mapped to UTF-8 characters
// before distance is calculated.
uint64_t l_dist(void *first_word, void *second_word) {
  // Map strings to UTF-8.
  Character *first = map_chr_str(first_word);
  Character *second = map_chr_str(second_word);
  uint64_t firstLen = chr_strlen(first);
  uint64_t secondLen = chr_strlen(second);

  // If either word is empty, we return early with the length of the other word.
  if (firstLen == 0) {
    return secondLen;
  }

  if (secondLen == 0) {
    return firstLen;
  }

  uint64_t dist[firstLen + 1][secondLen + 1];

  // Initialize the distance matrix
  // (That sounds way cooler than it actually is...)
  for (uint64_t i = 0; i <= firstLen; i++) {
    dist[i][0] = i;
  }

  for (uint64_t i = 0; i <= secondLen; i++) {
    dist[0][i] = i;
  }

  // Propagate the distance matrix with calculated edit distances for all characters.
  for (uint64_t i = 1; i <= firstLen; i++) {
    for (uint64_t j = 1; j <= secondLen; j++) {
      uint64_t match = chrcmp(first[i - 1], second[j - 1]) ? 1 : 0;

      dist[i][j] = min(min(dist[i][j - 1] + 1, dist[i - 1][j] + 1), dist[i - 1][j - 1] + match);
    }
  }

  free_chr_str(first);
  free_chr_str(second);

  uint64_t ret = dist[firstLen][secondLen];

  return ret;
}

// Returns the allocated memory, or exits with an error.
void *handle_calloc(uint64_t len, size_t size) {
  void *alloc = calloc(len, size);
  if (!alloc) {
    fprintf(stderr, "out of memory\n");
    exit(EXIT_FAILURE);
  }

  return alloc;
}
