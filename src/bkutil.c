#include "bkutil.h"
#include "character.h"

// TODO:0 Implement Jaccard Distance
// TODO:40 Handle Levenshtein Distance with n-grams
// TODO:30 Handle Jaccard Distance with n-grams
// TODO:20 Handle modified Jaccard Distance with n-grams

uint64_t MAX_HEX_HAM_DIST = 256;
uint64_t MAX_PERCENT_DIST = 100;


// Returns the hamming distance between two strings made of hex characters.
//
// Expected input string may contain:
//   * 0-9
//   * a-f
//   * A-F
uint64_t hex_ham_dist(void *first_hash, void *second_hash) {
  // If the hashes are empty, return a max distance.
  if (first_hash == NULL || second_hash == NULL) {
    return MAX_HEX_HAM_DIST;
  }

  uint8_t *first = first_hash;
  uint8_t *second = second_hash;
  uint64_t len1 = strlen(first);
  uint64_t len2 = strlen(second);

  // If the hashes are not equal in length, return a max distance.
  if (len1 != len2) {
    return MAX_HEX_HAM_DIST;
  }

  // Converts a hex character to it's decimal value.
  uint8_t hex_to_dec(uint8_t value) {
    // Check if the character is a valid hex character before attempting to convert it.
    if (
      value < 48 ||
      value > 57 && value < 65 ||
      value > 70 && value < 97 ||
      value > 102
    ) {
      // If the character is invalid for hex, we will error with hopefully a helpful message.
      fprintf(stderr, "Found unexpected character \"%c\" which is outside the range of hex values.\n", value);
      exit(EXIT_FAILURE);
    }

    uint out;
    sscanf(&value, "%x", &out);

    return (uint8_t) out;
  }

  // Returns the compared value of the hex character.
  uint64_t hex_char_cmp(uint8_t value1, uint8_t value2) {
    uint8_t val1 = hex_to_dec(value1);
    uint8_t val2 = hex_to_dec(value2);

    uint64_t sum = 0;

    // Find the binary value for 8 positions in binary array created by the hex value,
    // and add to the internal sum when they are different.
    for (uint64_t i = 0; i < 8; i++) {
      if ((val2 >> i & 1) != (val1 >> i & 1)) {
        sum += 1;
      }
    }

    return sum;
  }

  uint64_t sum = 0;

  // Find the sum of the difference between two characters in each position of both strings.
  for (uint64_t i = 0; i < len1; i++) {
    sum += hex_char_cmp(first[i], second[i]);
  }

  return sum;
}

// Returns Jaro Distance of two given strings multiplied by 100.  Strings are mapped to utf-8 characters
// before their distance is compared.
uint64_t jaro_dist(void *first_word, void *second_word) {
  // If either string given is empty, return max distance
  if (first_word == NULL || second_word == NULL) {
    return MAX_PERCENT_DIST;
  }

  // UTF-8 character mapping
  Character *first = map_chr_str(first_word);
  Character *second = map_chr_str(second_word);
  uint64_t len1 = chr_strlen(first);
  uint64_t len2 = chr_strlen(second);

  // If either string is empty after mapping, return max distance
  if (len1 == 0 || len2 == 0) {
    return MAX_PERCENT_DIST;
  }

  // Half is the max search distance within a string.
  uint64_t half = max(len1, len2) / 2;
  Character null = map_chr(NULL);

  // These variable names are single-letter to make the formula for distance not heinously long.
  // 'm' is for Matching characters
  uint64_t m = 0;
  // 't' is for Transpositioned characters
  uint64_t t = 0;

  for (uint64_t i = 0; i < len1; i++) {
    for (uint64_t j = 0; j < half && i + j < len2; j++) {
      // We check forwards first, if there is a match in the forwards direction, no transposition is needed.
      if (!chrcmp(first[i], second[i + j])) {
        m += 1;
        second[i + j] = null;

        // Only match once
        break;
      }

      // We check backwards for transpositions, but only after searching for max distance forwards.
      //
      // We will match a transposition character closer to the position of the original character
      // before attempting to match the next position for a normal match.
      //
      // This is to ensure that match position distances are minimized, so matches will be maximized.
      if (i >= j && !chrcmp(first[i], second[i - j])) {
        m += 1;
        t += 1;
        second[i - j] = null;

        // Only match once
        break;
      }
    }
  }

  free_chr_str(first);
  free_chr_str(second);

  return MAX_PERCENT_DIST - (MAX_PERCENT_DIST / 3 * ((float) m / len1 + (float) m / len2 + (float) (m - t) / m));
}

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
    return MAX_PERCENT_DIST;
  }

  // Map to UTF-8 characters
  Character *first = map_chr_str(first_word);
  Character *second = map_chr_str(second_word);
  uint64_t len1 = chr_strlen(first);
  uint64_t len2 = chr_strlen(second);

  if (len1 == 0 || len2 == 0) {
    return MAX_PERCENT_DIST;
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

  uint64_t dist = MAX_PERCENT_DIST - ((MAX_PERCENT_DIST * intersect_count) / union_count);

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
