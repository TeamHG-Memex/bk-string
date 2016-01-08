#ifndef BK_NODE_H
#define BK_NODE_H
#include <stdlib.h>

// BK Nodes have 32 children, because it is unlikely that a word will be further
// than 31 in Levenshtein Distance.  We can increase this amount or dynamically
// grow the child node array if this is found to be unsuitable.
const int CHILD_NODES = 32;

typedef struct BKNode BKNode;
struct BKNode {
  char *word;
  BKNode *child;
  int empty;
  int size;

  void (*AddChild) (char *, BKNode *);
};

// Adds a word to a given node.
void add_child(char *word, BKNode *self) {
  self->word = word;
  self->empty = 0;
};

// Returns an initialized BK Node
BKNode init_bknode() {
  BKNode node;
  node.empty = 1;
  node.size = CHILD_NODES;
  node.child = malloc(sizeof(BKNode) * CHILD_NODES);

  // Set each child node to "empty."
  for (int i = 0; i < CHILD_NODES; i++) {
    node.child[i].empty = 1;
  }

  node.AddChild = add_child;

  return node;
};
#endif
