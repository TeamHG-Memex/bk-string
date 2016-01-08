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

#ifndef BK_H
#define BK_H

#include "./lib/bknode.c"
#include "./lib/bkutil.c"
#include "./lib/bkslist.c"

typedef struct BKTree BKTree;
struct BKTree {
  BKNode _root;
  SearchList _list;
  void (*Add) (char *, BKTree *);
  char** (*Search) (char *, int, BKTree*);
};

// A function to add a word to a BK Tree.
void bk_add(char *word, BKTree *self) {
  // Check the root for an entry.
  if (self->_root.empty) {
    self->_root.empty = 0;
    self->_root.word = word;

    return;
  }

  // Initialize vars needed for tree buildling.
  BKNode *curNode = &self->_root;
  int dist = l_dist(curNode->word, word);

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
void r_search(char *word, int dist, SearchList *s_list, BKNode *node) {
  if (node->empty) {
    return;
  }

  int curDist = l_dist(node->word, word);

  // If current distance is less than the search distance:
  if (curDist <= dist) {
    // Add that word to the search list.
    list_add(node->word, s_list);
  }

  int minDist = curDist - dist;
  int maxDist = curDist + dist;

  // Loop through all node children
  for (int i = 0; i <= node->size; i++) {
    // If the distance of the child at "i" is within the allowable distance from
    // the search word:
    if (i >= minDist && i <= maxDist && !node->child[i].empty) {
      // Search the child's children for additional words.
      r_search(word, dist, s_list, &node->child[i]);
    }
  }
};

// Begin a search on a given BK Tree
char** search(char *word, int dist, BKTree *b) {
  BKNode *node = &b->_root;
  SearchList* s_list = &b->_list;

  r_search(word, dist, s_list, node);

  return s_list->list;
};

// Returns an initialized BK Tree.
BKTree init_bk_tree() {
  BKTree b;
  b._root = init_bknode();
  b._list = init_search_list();

  b.Add = bk_add;
  b.Search = search;

  return b;
};

#endif
