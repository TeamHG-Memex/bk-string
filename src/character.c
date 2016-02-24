#include "character.h"

#include <assert.h>

// Returns an initialized empty character.
Character init_chr() {
  Character chr;
  chr.empty = 1;

  for (uint64_t i = 0; i < 4; i++) {
    chr.bytes[i] = 0;
  }

  return chr;
}

// Checks the given byte has a continuation for UTF-8 handling.
uint8_t is_not_continued_byte(uint8_t byte) {
  // http://stackoverflow.com/questions/1682996/bytes-to-binary-in-c
  return ((byte & 0xc0) != 0x80) ? 1 : 0;
}

// Compares 2 char byte arrays, returns -1 or 1 if they don't match depending, and 0 if they do.
// 1 is returned if the first mismatched byte of the first character is greater than the byte at
// the same index in the second character.
int64_t chrcmp(Character chr1, Character chr2) {
  if (chr1.empty && chr2.empty) {
    // Exit early if the characters are defined as empty.
    return 0;
  }

  if (chr1.empty) {
    return -1;
  }

  if (chr2.empty) {
    return 1;
  }

  for (uint8_t i = 0; i < 4; i++) {
    if (chr1.bytes[i] != chr2.bytes[i]) {
      if (chr1.bytes[i] > chr2.bytes[i]) {
        // Return 1 if the mismatched byte of the first character is greater than the byte
        // in the same location for the second character.
        return 1;
      }
      // Otherwise return -1.
      return -1;
    }
  }

  // Default return is 0.
  return 0;
}

// Returns the first character of a given string.
Character map_chr(void *bytes) {
  Character chr = init_chr();

  // We do a special check to see if "bytes" is not an array of bytes, and instead
  // is just NULL.  In these cases, we return an empty character.

  // This check is not attempting to compare "bytes" to a NULL character or "\0."

  // The reason we handle NULL specifically as the array definition instead of a byte
  // array with NULL in it, is because UTF-8 considers NULL a valid character.
  if (bytes == NULL) {
    return chr;
  }

  uint8_t *byte_arr = bytes;

  // The first byte is always stored.
  chr.bytes[0] = byte_arr[0];

  for (uint64_t i = 1; i < 4; i++) {
    if (is_not_continued_byte(byte_arr[i])) {
      break;
    }
    chr.bytes[i] = byte_arr[i];
  }

  chr.empty = 0;

  return chr;
}

// Resets a character, since characters do not allocate memory to be freed.
void free_chr(Character *chr) {
  for (uint64_t i = 0; i < 4; i++) {
    chr->bytes[i] = 0;
  }

  chr->empty = 1;
}

// Frees all memory used by a character string.
void free_chr_str(Character *str) {
  uint64_t i = 0;
  Character null = map_chr(NULL);

  while (chrcmp(str[i], null)) {
    free_chr(&str[i]);
    i += 1;
  }

  free_chr(&null);
  free(str);
}

// Counts bytes in a string which are not continuation bytes
uint64_t count_non_continued_bytes(void *str) {
  uint8_t *string = str;
  uint8_t length = strlen(string);
  uint64_t count = 0;

  for (uint64_t i = 0; i < length; i++) {
    if (is_not_continued_byte(string[i])) {
      count += 1;
    }
  }

  return count;
}

// Returns a Character array which is terminated by a null Character.
Character *map_chr_str(void *str) {
  uint8_t *string = str;
  uint64_t length = count_non_continued_bytes(string) + 1; // Add 1 for the terminating byte.
  Character *ret = calloc(length, sizeof(Character));

  assert(ret != NULL);

  uint8_t byte_arr[4] = { 0 };
  uint8_t byte_idx = 0;
  uint64_t str_idx = 0;

  // Super secret internal helper function for resetting the byte array.
  void reset_byte_arr(uint8_t *bytes) {
    for (uint64_t i = 0; i < 4; i++) {
      bytes[i] = 0;
    }
  }

  // Loop through all bytes in the string, collect bytes in an array to be stored as a
  // single character.
  //
  // When the current byte is the next non-continued byte, store the current byte array as
  // a character in the string, and then reset the byte array for the next character.
  for (uint64_t i = 0; i < strlen(string); i++) {
    byte_arr[byte_idx] = string[i];
    byte_idx += 1;

    if (is_not_continued_byte(string[i + 1])) {
      // Store character in string and then reset byte array for next character.

      // Store the current character from the byte array
      ret[str_idx] = map_chr(byte_arr);
      str_idx += 1;

      reset_byte_arr(byte_arr);
      byte_idx = 0;
    }
  }

  // NULL terminate to make it more C-like string.  I am not certain this is the
  // best way to handle Character strings, but it at least is consistent with the C
  // implementation of strings.
  ret[length - 1] = map_chr(NULL);

  return ret;
}

// Returns the length of a Character string, not including the terminating Character.
uint64_t chr_strlen(Character *str) {
  uint64_t len = 0;
  Character null = map_chr(NULL);

  while (chrcmp(str[len], null)) {
    len += 1;
  }

  free_chr(&null);

  return len;
}

// Compares two strings, returns 0 if the strings match.  Returns -1 or 1 if the
// first mismatched Character is less or greater in the first string as compared
// to the same Character in the same index on the second string.

// Example:
// chr_strcmp("foo", "bar") == 1  ("f" is greater than "b.")
// chr_strcmp("bar", "foo") == -1 ("b" is less than "f.")
// chr_strcmp("foo", "foo") == 0
int64_t chr_strcmp(Character *str1, Character *str2) {
  uint64_t len1 = chr_strlen(str1);
  uint64_t len2 = chr_strlen(str2);

  for (uint64_t i = 0; i < len1 && i < len2; i++) {
    int64_t notMatch = chrcmp(str1[i], str2[i]);
    if (notMatch) {
      return notMatch;
    }
  }

  return 0;
}

// TODO:50 Implement "map_byte_str()" which will map a character string to a uint8_t byte array.
