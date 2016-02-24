// Usage

// To initialize a BK Tree for strings:
//   BKTree b = init_bk_tree();

// To add words to the BK Tree:
//   b.Add("foo", &b);

// To search the BK Tree for a list of words within a given distance:
//   char** list = b.Search(char* word, int distance, BKTree* b);
//   assert(!strcmp(list[0], "foo"));

// Don't forget to free your search list when you're done with it!
//   free(list);


// A Thank You:
// This implementation was adapted from a C# implementation found on the internet.
// Specifically a blog post by Jerry Bachmann on using BK Trees for Spell Checking.
// Here's a link:
// https://nullwords.wordpress.com/2013/03/13/the-bk-tree-a-data-structure-for-spell-checking/

// Thanks Jerry!

#include "bkutil.h"
#include "bkslist.h"
#include "bkstring.h"

// TODO:70 Add the ability to add an array of strings to the BK Tree.

// A function to add a word to a BK Tree.
void bk_add(void *word_arg, BKTree *self) {
  uint8_t *word = word_arg;

  // Check the root for an entry.
  if (self->_root.empty == 1) {
    self->_root.AddChild(word, &self->_root);

    return;
  }

  // Initialize vars needed for tree buildling.
  BKNode *curNode = &self->_root;
  uint64_t dist = self->Dist(curNode->word, word);

  if (dist == 0) {
    // If "dist" is "0," the word is already stored, so it won't be added again.
    return;
  }

  while (!curNode->child[dist].empty) {
    curNode = &curNode->child[dist];
    dist = self->Dist(curNode->word, word);

    if (dist == 0) {
      // If "dist" is "0," the word is already stored, so it won't be added again.
      return;
    }
  }

  // Since the current child is empty, we must initialize it.
  curNode->child[dist] = init_bknode();
  curNode = &curNode->child[dist];

  // Add child values.
  curNode->AddChild(word, curNode);
};

// Recursively search a Node and add the search results to a given Search List.
void r_search(void *word_arg, uint64_t dist, SearchList *s_list, BKNode *node, uint64_t (*dist_function) (void *, void *)) {
  if (node->empty) {
    return;
  }

  uint8_t *word = word_arg;
  uint64_t curDist = dist_function(node->word, word);
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
      r_search(word, dist, s_list, &node->child[i], dist_function);
    }
  }
};

// Begin a search on a given BK Tree
void* search(void *word_arg, uint64_t dist, BKTree *b) {
  uint8_t *word = word_arg;
  BKNode *node = &b->_root;
  SearchList s_list = init_search_list();

  r_search(word, dist, &s_list, node, b->Dist);

  // Ensure the Search List terminates with NULL, so users can predictably find
  // the end.
  list_add(NULL, &s_list);

  return s_list.list;
};

// Returns an initialized BK Tree.
BKTree new_bktree(void *dist_function) {
  BKTree b;
  b._root = init_bknode();

  if (dist_function == NULL) {
    dist_function = l_dist;
  }

  b.Add = bk_add;
  b.Search = search;
  b.Dist = dist_function;

  return b;
};

BKTree init_bktree() {
  return new_bktree(NULL);
}

// Procedural function to set a BK Tree to initialization values
void init(BKTree *b, void *dist_function) {
  *(b) = new_bktree(dist_function);
}

// Deallocates the BK Tree
void clear_bktree(BKTree *bk) {
  clear_bknode(bk->_root);
}
