// Usage

// To initialize a BK Tree for strings:
//   BKTree b = init_bk_tree();

// To add words to the BK Tree:
//   b.Add("foo", &b);

// To search the BK Tree for a list of words within a given distance:
//   char** list = b.Search(char* word, int distance, BKTree* b);
//   assert(list[0] == "foo");


// A Thank You:
// This implementation was adapted from a C# implementation found on the internet.
// Specifically a blog post by Jerry Bachmann on using BK Trees for Spell Checking.
// Here's a link:
// https://nullwords.wordpress.com/2013/03/13/the-bk-tree-a-data-structure-for-spell-checking/

// Thanks Jerry!

#include "bkutil.h"
#include "bkslist.h"
#include "bkstring.h"

// DOING:0 Create a makefile.
// TODO:20 Create a header file to import BK Tree.
// TODO:10 Add the ability to add an array of strings to the BK Tree.
// TODO:0 Add python wrapper for the following BK Tree functionality:
//      .Add()
//      .Search()
//      init_bk_tree()
//      free_bk_tree()

// A function to add a word to a BK Tree.
void bk_add(void *word_arg, BKTree *self) {

  uint8_t *word = word_arg;
  // Check the root for an entry.
  if (self->_root.empty == 1) {
    self->_root.word = word;
    self->_root.empty = 0;

    return;
  }

  // Initialize vars needed for tree buildling.
  BKNode *curNode = &self->_root;
  size_t dist = l_dist(curNode->word, word);

  while (!curNode->child[dist].empty) {
    if (dist == 0) {
      // If "dist" is "0," the word is already stored, so it won't be added again.
      return;
    }

    curNode = &curNode->child[dist];
    dist = l_dist(curNode->word, word);
  }

  // Since the current child is empty, we must initialize it.
  curNode->child[dist] = init_bknode();
  curNode = &curNode->child[dist];

  // Add child values.
  curNode->AddChild(word, curNode);
};

// Recursively search a Node and add the search results to a given Search List.
void r_search(void *word_arg, uint64_t dist, SearchList *s_list, BKNode *node) {
  if (node->empty) {
    return;
  }

  uint8_t *word = word_arg;
  uint64_t curDist = l_dist(node->word, word);
  uint64_t minDist;

  // If current distance is less than the search distance:
  if (curDist <= dist) {
    // Add that word to the search list.
    list_add(node->word, s_list);

    // minDist is set to 0 because it is an unsigned int.
    minDist = 0;
  } else {
    // minDist is set to the minimum distance allowed from the current word.
    minDist = curDist - dist;
  }

  uint64_t maxDist = curDist + dist;

  // Loop through all node children
  for (uint64_t i = 0; i <= node->size; i++) {
    // If the distance of the child at "i" is within the allowable distance from
    // the search word:
    if (i >= minDist && i <= maxDist && !node->child[i].empty) {
      // Search the child's children for additional words.
      r_search(word, dist, s_list, &node->child[i]);
    }
  }
};

// Begin a search on a given BK Tree
void* search(void *word_arg, uint64_t dist, BKTree *b) {
  uint8_t *word = word_arg;
  BKNode *node = &b->_root;
  SearchList s_list = init_search_list();

  r_search(word, dist, &s_list, node);

  // Ensure the Search List terminates with NULL, so users can predictably find
  // the end.
  list_add(NULL, &s_list);

  return s_list.list;
};

// Returns an initialized BK Tree.
BKTree init_bktree() {
  BKTree b;
  b._root = init_bknode();

  b.Add = bk_add;
  b.Search = search;

  return b;
};

// Deallocates the BK Tree
void clear_bktree(BKTree *bk) {
  clear_bknode(bk->_root);
}
