#ifndef CHARACTER
#define CHARACTER

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Character Character;
struct Character {
  uint8_t bytes[4];
  uint8_t empty;
};

uint8_t is_not_continued_byte(uint8_t byte);

extern int64_t chrcmp(Character c1, Character c2);
extern int64_t chr_strcmp(Character *str1, Character *str2);
extern uint64_t chr_strlen(Character *str);
extern Character map_chr(void *bytes);
extern Character *map_chr_str(void *str);
extern uint8_t *map_byte_str(Character *str);
extern void free_chr(Character *chr);
extern void free_chr_str(Character *str);

#endif
