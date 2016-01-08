#include "../bkstring.c"
#include <assert.h>
#include <stdio.h>

void test() {
  // Test the BK Tree's "Add" method.
  BKTree b = init_bk_tree();
  b.Add("foo", &b);
  assert(b._root.word == "foo");
  b.Add("bar", &b);
  assert(b._root.child[3].word == "bar");
  b.Add("baz", &b);
  assert(b._root.child[3].child[1].word == "baz");


  // Test the Levenshtein Distance function.
  // Test that it is returning the correct distance, and not off by 1.
  int dist = l_dist("foo", "bar");

  assert(dist == 3);
  assert(dist != 2);

  // Test that it gives the same distance for two words given in either order.
  dist = l_dist("bar", "baz");
  assert(dist == 1);

  dist = l_dist("baz", "bar");
  assert(dist == 1);

  // Test the BK search function
  // Add 2 more members to the BK tree to ensure the search must expand
  // the list past the original allocation.
  b.Add("bam", &b);
  b.Add("bat", &b);

  char** list = b.Search("bar", 2, &b);

  // Test all the values given to the BK tree.
  assert(list[0] = "bar");
  assert(list[1] = "baz");
  assert(list[2] = "bam");
  assert(list[3] = "bat");
}

int main() {
  test();

  return 0;
};
