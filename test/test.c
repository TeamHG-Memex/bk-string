#include <assert.h>
#include <stdbool.h>

#include "bkstring.h"
#include "bkutil.h"

// Returns true if a list has a word in it.
bool has_word(void* list_arg, void* word_arg) {
  uint8_t **list = list_arg;
  uint8_t *word = word_arg;

  size_t i = 0;

  // This is a naive way of interpreting whether a list has no more members.
  // This causes a segmentation fault if the list is full to the last index.
  while (list[i] != NULL) {
    if (!strcmp(list[i], word)) {
      return true;
    }

    i += 1;
  }

  return false;
}

uint64_t inject(void *first, void *second) {
  return 1;
}

void test() {
  printf("Testing continued byte check: ");
  // It should contain no continuation bits on single-byte unicode characters.
  uint8_t *str = "a";
  assert(is_not_continued_byte(str[0]));
  assert(is_not_continued_byte(str[1]));
  assert(is_not_continued_byte(str[2]));
  assert(is_not_continued_byte(str[3]));
  str = NULL;

  // It should contain continuation bits on multi-byte unicode characters.
  str = "\u263a";
  assert(is_not_continued_byte(str[0]));
  assert(!is_not_continued_byte(str[1]));
  assert(!is_not_continued_byte(str[2]));
  assert(is_not_continued_byte(str[3]));

  printf("\u2714\n");


  printf("Testing character compare: ");
  // It should match when the character byte arrays match.
  uint8_t char1[4] = {1, 1, 1, 1};
  uint8_t char2[4] = {1, 1, 1, 1};

  assert(!char_cmp(char1, char2));

  // It should not match when one of the character byte arrays is changed.
  char2[3] = 0;

  assert(char_cmp(char1, char2));

  printf("\u2714\n");


  printf("Testing injection of distance function in BK Tree: ");
  // "inject" is a dummy function which always returns 1.
  BKTree b = new_bktree(inject);
  b.Add("foo", &b);
  b.Add("bar", &b);

  // It should return 1 regardless of the strings, and correctly store the words in the BK Tree.
  assert(b.Dist("foo", "bar") == 1);
  assert(!strcmp(b._root.child[1].word, "bar"));

  clear_bktree(&b);

  // It should initialize by default with Levenshtein Distance.
  b = init_bktree();

  // It should correctly use Levenshtein Distance.
  assert(b.Dist("foo", "bar") == 3);

  clear_bktree(&b);
  printf("\u2714\n");


  printf("Testing Levenshtein Distance: ");
  // Test the Levenshtein Distance function.
  // Test that it is returning the correct distance, and not off by 1.
  assert(l_dist("foo", "bar") == 3);
  assert(l_dist("foo", "bar") != 2);

  // Test that it gives the same distance for two words given in either order.
  // printf("dist: %zu\n", l_dist("foo", "foodingding"));
  assert(l_dist("bar", "baz") == 1);
  assert(l_dist("baz", "bar") == 1);

  // Test that a duplicate word gives a distance of 0.
  assert(l_dist("foo", "foo") == 0);
  assert(l_dist("bar", "bar") == 0);

  // Test for distances of differing sized words.
  assert(l_dist("foo", "fooding") == 4);
  assert(l_dist("foo", "foodingding") == 8);

  // Test for distances with unicode characters.
  assert(l_dist("johndoe1", "johndoe\u263a") == 1);
  assert(l_dist("johndoe1", "johndoe\u263a1") == 1);
  assert(l_dist("johndoe1", "johndoe\u263a\u263a") == 2);
  assert(l_dist("johndoe\u263a", "johndoe1") == 1);

  printf("\u2714\n");


  printf("Testing addition of items in BK Tree: ");
  // Test the BK Tree's "Add" method.
  b = init_bktree();
  b.Add("foo", &b);
  assert(!strcmp(b._root.word, "foo"));
  b.Add("bar", &b);
  assert(!strcmp(b._root.child[3].word, "bar"));
  b.Add("baz", &b);
  assert(!strcmp(b._root.child[3].child[1].word, "baz"));

  b.Add("bar", &b);
  assert(!strcmp(b._root.word, "foo"));
  printf("\u2714\n");

  printf("Testing BK Search: ");
  // Test the BK search function
  // Add more members to the BK tree to ensure the search must expand
  // the list past the original allocation.
  b.Add("bam", &b);
  b.Add("bat", &b);
  b.Add("baw", &b);
  b.Add("bae", &b);
  b.Add("bay", &b);
  b.Add("bau", &b);
  b.Add("bai", &b);
  b.Add("bao", &b);
  b.Add("bap", &b);
  b.Add("baa", &b);
  b.Add("bas", &b);
  b.Add("bad", &b);
  b.Add("baf", &b);
  b.Add("bag", &b);
  b.Add("bah", &b);
  b.Add("baj", &b);
  b.Add("bak", &b);
  b.Add("bal", &b);
  b.Add("bax", &b);
  b.Add("bac", &b);
  b.Add("bav", &b);
  b.Add("bab", &b);
  b.Add("ban", &b);

  uint8_t** list = b.Search("bar", 2, &b);
  clear_bktree(&b);

  // Test all the values given to the BK tree.
  assert(has_word(list, "bar"));
  assert(has_word(list, "bat"));
  assert(has_word(list, "baz"));
  assert(has_word(list, "ban"));

  // Check to make sure tests are not passing by default.
  assert(!has_word(list, "bowowowowow"));

  free(list);
  list = NULL;
  printf("\u2714\n");

  // Testing addition via array
  printf("Testing addtion/searching via array: ");
  b = init_bktree();
  uint8_t *arr[10] = {"foo", "bar", "baz", "bat", "food", "fan", "far", "bufar", "bingo", "alala"};

  for (uint64_t i = 0; i < 10; i++) {
    b.Add(arr[i], &b);

    assert(!strcmp(b._root.word, "foo"));
  }

  list = b.Search("bar", 1, &b);

  assert(has_word(list, "far"));

  // Test to make sure the previous BK Tree was de-allocated appropriately.
  assert(!has_word(list, "ban"));

  // Check for NULL terminator on the search list.
  assert(list[11] == NULL);

  free(list);
  list = NULL;

  list = b.Search("al", 3, &b);

  assert(has_word(list, "alala"));
  assert(!has_word(list, "bufar"));
  assert(list[7] == NULL);

  clear_bktree(&b);

  free(list);
  list = NULL;

  printf("\u2714\n");
}

int main() {
  test();

  return 0;
};
