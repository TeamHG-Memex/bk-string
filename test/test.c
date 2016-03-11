#include <assert.h>
#include <stdbool.h>

#include "bkstring.h"
#include "bkutil.h"
#include "character.h"

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

void test_character_lib() {
  printf("\n=========== Testing Character Library ================\n");

  printf(" Testing map_chr: ");

  Character chr = map_chr("\0");

  assert(!chr.empty);

  free_chr(&chr);

  assert(chr.empty);

  chr = map_chr("a");

  assert(!chr.empty);

  free_chr(&chr);

  assert(chr.empty);

  chr = map_chr("ab");

  assert(chr.bytes[0] == 'a');
  assert(chr.bytes[1] != 'b');
  assert(chr.bytes[1] == 0);

  free_chr(&chr);

  printf("\u2714\n");


  printf(" Testing chrcmp: ");

  Character chr1 = map_chr("\u263a");
  Character chr2 = map_chr("\u263a");

  assert(!chrcmp(chr1, chr2));

  free_chr(&chr1);
  free_chr(&chr2);

  chr1 = map_chr("\u263a");
  chr2 = map_chr("\u263b");

  // It should return a length similarly to how
  assert(chrcmp(chr1, chr2) < 0);
  assert(chrcmp(chr2, chr1) > 0);

  free_chr(&chr1);
  free_chr(&chr2);

  printf("\u2714\n");


  printf(" Testing string compare of character strings: ");

  // It should return a negative number if the bytes from the first string are
  // less than the bytes from the second.  It should return a positive number if
  // the bytes from the first are greater than the bytes from the second.
  Character *w1 = map_chr_str("foo");
  Character *w2 = map_chr_str("bar");

  assert(chr_strcmp(w1, w2) > 0);
  assert(chr_strcmp(w2, w1) < 0);

  free_chr_str(w1);
  free_chr_str(w2);

  // It should always return 0 regardless of order of arguments if the character strings are equal.
  w1 = map_chr_str("foo");
  w2 = map_chr_str("foo");

  assert(chr_strcmp(w1, w2) == 0);
  assert(chr_strcmp(w2, w1) == 0);

  free_chr_str(w1);
  free_chr_str(w2);

  // It should always return 0 regardless of order of arguments if the character strings are equal.
  w1 = map_chr_str("oof");
  w2 = map_chr_str("foo");

  assert(chr_strcmp(w1, w2) != 0);
  assert(chr_strcmp(w2, w1) != 0);

  free_chr_str(w1);
  free_chr_str(w2);

  printf("\u2714\n");


  printf(" Testing continued byte check: ");
  // It should contain no continuation bits on single-byte unicode characters.
  uint8_t *str = "a";
  assert(is_not_continued_byte(str[0]));
  assert(is_not_continued_byte(str[1]));
  assert(str[1] == 0);
  str = NULL;

  // It should contain continuation bits on multi-byte unicode characters.
  str = "\u263a";
  assert(is_not_continued_byte(str[0]));
  assert(!is_not_continued_byte(str[1]));
  assert(!is_not_continued_byte(str[2]));
  assert(is_not_continued_byte(str[3]));
  assert(str[3] == 0);

  printf("\u2714\n");


  printf(" Testing character strings: ");

  str = "stuff and \u263a";

  assert(strlen(str) == 13);

  Character *chr_str = map_chr_str(str);

  assert(chr_strlen(chr_str) == 11);

  free_chr_str(chr_str);

  printf("\u2714\n");

  // TODO:60 Make the test below work after implementing a character string to byte array mapping function.
  // printf("Testing from character string to byte string: ");
  //
  // str = "foo";
  // chr_str = map_chr_str(str);
  //
  // assert(map_byte_str(chr_str) == str);
  //
  // printf("\u2714\n");
}


void test_bkutil_lib() {
  printf("\n=========== Testing BK Util Library ==================\n");

  printf(" Testing Levenshtein Distance: ");
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

  // Test for distances with unicode characters.
  assert(l_dist("johndoe1", "johndoe\u263a") == 1);
  assert(l_dist("johndoe1", "johndoe\u263a1") == 1);
  assert(l_dist("johndoe1", "johndoe\u263a\u263a") == 2);
  assert(l_dist("johndoe\u263a", "johndoe1") == 1);

  printf("\u2714\n");


  printf(" Testing Modified Jaccard Distance: ");

  assert(mod_j_dist("foo", "bar") == 100);
  assert(mod_j_dist("bar", "ba") == 34);
  assert(mod_j_dist("bar", "baz") == 50);
  assert(mod_j_dist("GG", "GGGG") == 50);
  assert(mod_j_dist("fooba 1234", "fooba1234") == 10);

  printf("\u2714\n");


  printf(" Testing Hex Hamming Distance: ");
  // TODO:10 Failing tests for hex_ham_dist after official unit test suite is in place.

  // A bunch of sample hamming distances to compute.  All should match.
  assert(hex_ham_dist("0590eb7e1129fa5b", "435e9db1634baca2") == 36);
  assert(hex_ham_dist("0590eb7e1129fa5b", "e13c832b7ce2720f") == 30);
  assert(hex_ham_dist("0590eb7e1129fa5b", "cd87c969b794125a") == 28);
  assert(hex_ham_dist("0590eb7e1129fa5b", "096d864c93b396b7") == 32);
  assert(hex_ham_dist("0590eb7e1129fa5b", "6dc3693d11d0da4b") == 20);
  assert(hex_ham_dist("0590eb7e1129fa5b", "4f6ad94847cd2539") == 34);
  assert(hex_ham_dist("0590eb7e1129fa5b", "33edac42c731b135") == 34);
  assert(hex_ham_dist("0590eb7e1129fa5b", "9327939737447c1c") == 34);
  assert(hex_ham_dist("0590eb7e1129fa5b", "5fa9e49021de9176") == 36);
  assert(hex_ham_dist("0590eb7e1129fa5b", "a991569a1a66ed99") == 30);
  assert(hex_ham_dist("0590eb7e1129fa5b", "eb90ed295a62b465") == 30);
  assert(hex_ham_dist("0590eb7e1129fa5b", "4d67581a3f97283c") == 36);
  assert(hex_ham_dist("0590eb7e1129fa5b", "87506bfe01a4f84f") == 14);
  assert(hex_ham_dist("0590eb7e1129fa5b", "9fade410215e517e") == 34);
  assert(hex_ham_dist("0590eb7e1129fa5b", "d7bbcb6c3a369040") == 28);

  assert(hex_ham_dist("0590EB7E1129FA5B", "D7BBCB6C3A369040") == 28);

  printf("\u2714\n");


  printf(" Testing Jaro Distance: ");
  assert(jaro_dist("duane", "dwayne") == 18);
  assert(jaro_dist("martha", "marhta") == 6);
  assert(jaro_dist("dixon", "dicksonx") == 24);
  assert(jaro_dist("1234567@", "12354@") == 14);

  printf("\u2714\n");
}

void test_bktree_lib() {
  printf("\n=========== Testing BK Tree Library ==================\n");

  printf(" Testing injection of distance function in BK Tree: ");
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


  printf(" Testing addition of items in BK Tree: ");
  // Test the BK Tree's "Add" method.
  b = init_bktree();
  b.Add("foo", &b);
  assert(!strcmp(b._root.word, "foo"));
  assert(has_word(b.Search("fod", 1, &b), "foo"));
  b.Add("bar", &b);
  assert(!strcmp(b._root.child[3].word, "bar"));
  b.Add("baz", &b);
  assert(!strcmp(b._root.child[3].child[1].word, "baz"));

  b.Add("bar", &b);
  assert(!strcmp(b._root.word, "foo"));
  printf("\u2714\n");

  printf(" Testing BK Search: ");
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

  // Test that list has found the 24 words added.
  uint64_t idx = 1;

  while (list[idx + 1] != NULL) {
    idx += 1;
  }
  assert(idx == 24);

  // Test some of the values given to the BK tree.
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
  printf(" Testing addtion/searching via array: ");
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

  printf(" Specific Tests: ");
  b = init_bktree();
  b.Add("arromancy300", &b);

  assert(has_word(b.Search("arromancy", 3, &b), "arromancy300"));
  clear_bktree(&b);
  printf("\u2714\n");
}

void test() {
  test_character_lib();
  test_bkutil_lib();
  test_bktree_lib();
  printf("\n Tests Completed!\n\n");
}

int main() {
  test();

  return 0;
};
