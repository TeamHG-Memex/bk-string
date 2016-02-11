#include "bkutil.h"

// TODO: The unicode handling could use some refactoring and abstraction.

// Checks the given byte has a continuation for UTF-8 handling.
uint8_t is_not_continued_byte(uint8_t byte) {
  // http://stackoverflow.com/questions/1682996/bytes-to-binary-in-c
  return ((byte & 0xc0) != 0x80) ? 1 : 0;
}

// Resets the characters from an array of bytes representing a character.
void reset_char(uint8_t *bytes) {
  for (uint8_t i = 0; i < 4; i++) {
    bytes[i] = 0;
  }
}

// Compares 2 char byte arrays, returns 1 if they don't match and returns 0 if they do.
// The return values are consistent with "strcmp."
uint8_t char_cmp(uint8_t *char1, uint8_t *char2) {
  for (uint8_t i = 0; i < 4; i++) {
    if (char1[i] != char2[i]) {
      return 1;
    }
  }

  return 0;
}

// Sets the "c_arr" (character array) to the next character's bytes for the given word, and then returns
// a new value for the "w_idx" (word index).
uint64_t iter_char(uint8_t *c_arr, uint8_t *word, uint64_t w_idx, uint64_t length) {
  uint64_t c_idx = 0;
  reset_char(c_arr);

  // The first byte will always be a non-continued byte.
  c_arr[c_idx] = word[w_idx - 1];

  for (w_idx; w_idx < length; w_idx++) {
    // If the current byte is non-continued byte or if the character byte array is full:
    if (is_not_continued_byte(word[w_idx]) || c_idx > 3) {
      return w_idx;
    }

    c_idx += 1;
    c_arr[c_idx] = word[w_idx];
  }

  return w_idx;
}

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

  uint64_t dist[firstLen + 1][secondLen + 1];

  for (uint64_t i = 0; i <= firstLen; i++) {
    dist[i][0] = i;
  }

  for (uint64_t i = 0; i <= secondLen; i++) {
    dist[0][i] = i;
  }

  // TODO: Clean up or separate out the complex parts below.
  // Annoyingly C defines "char," though it is misnamed. So, we will create byte arrays
  // which will represent the full character given by multi-byte UTF-8 strings.

  // In order to handle unicode strings, we will need to iterate through both characters and
  // bytes.  This will require us to have indexes for characters, so that the distance arrays
  // are properly recorded, and indexes for the bytes in the strings, so that we can find characters.

  // Initialize character byte arrays.  They should never hold more than 4 bytes each for UTF-8.
  uint8_t c1[4] = {0, 0, 0, 0};
  uint8_t c2[4] = {0, 0, 0, 0};

  // Initialize indexes to be used, since we have to track counting both characters and bytes.
  uint64_t char_idx1 = 0;
  uint64_t char_idx2 = 0;
  uint64_t i_after = 0;
  uint64_t j_after = 0;

  for (uint64_t i = 1; i <= firstLen; i++) {
    char_idx1 += 1;
    char_idx2 = 0;

    i_after = iter_char(c1, first, i, firstLen);

    for (uint64_t j = 1; j <= secondLen; j++) {
      char_idx2 += 1;

      j_after = iter_char(c2, second, j, secondLen);
      uint64_t match = char_cmp(c1, c2);

      dist[char_idx1][char_idx2] =
        min(
          min(
            dist[char_idx1][char_idx2 - 1] + 1,
            dist[char_idx1 - 1][char_idx2] + 1
          ),
          dist[char_idx1 - 1][char_idx2 - 1] + match
        );

      if (j_after != j) {
        j = j_after - 1;
        char_idx2 -= 1;
      }
    }

    if (i_after != i) {
      i = i_after - 1;
      char_idx1 -= 1;
    }
  }

  uint64_t res = dist[char_idx1][char_idx2];

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
