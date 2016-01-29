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
  printf("Testing injection of distance function in BK Tree.\n");
  BKTree b = new_bktree(inject);
  b.Add("foo", &b);
  b.Add("bar", &b);

  assert(b.Dist("foo", "bar") == 1);
  assert(!strcmp(b._root.child[1].word, "bar"));

  clear_bktree(&b);

  b = init_bktree();

  assert(b.Dist("foo", "bar") == 3);

  clear_bktree(&b);
  printf("OK!\n");

  printf("Testing addition of items in BK Tree.\n");
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
  printf("OK!\n");

  printf("Testing Levenshtein Distance.\n");
  // Test the Levenshtein Distance function.
  // Test that it is returning the correct distance, and not off by 1.
  assert(l_dist("foo", "bar") == 3);
  assert(l_dist("foo", "bar") != 2);

  // Test that it gives the same distance for two words given in either order.
  assert(l_dist("bar", "baz") == 1);
  assert(l_dist("baz", "bar") == 1);

  // Test that a duplicate word gives a distance of 0.
  assert(l_dist("foo", "foo") == 0);
  assert(l_dist("bar", "bar") == 0);

  // Test for distances of differing sized words.
  assert(l_dist("foo", "fooding") == 4);
  assert(l_dist("foo", "foodingding") == 8);

  printf("OK!\n");

  printf("Testing BK Search.\n");
  // Test the BK search function
  // Add 2 more members to the BK tree to ensure the search must expand
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
  printf("OK!\n");

  // Testing addition via array
  printf("Testing addtion/searching via array.\n");
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
  printf("OK!\n");
}

int main() {
  test();

  return 0;
};
