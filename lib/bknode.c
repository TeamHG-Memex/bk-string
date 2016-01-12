#ifndef BK_NODE_H
#define BK_NODE_H
#include <stdlib.h>
#include <stdint.h>

#include "bkutil.c"

// BK Nodes have 256 children, because it is unlikely that a word will be further
// than 255 in Levenshtein Distance.  We can increase this amount or dynamically
// grow the child node array if this is found to be unsuitable.
const uint64_t CHILD_NODES = 256;

typedef struct BKNode BKNode;
struct BKNode {
  uint8_t *word;
  BKNode *child;
  uint64_t empty;
  uint64_t size;

  void (*AddChild) (void *, BKNode *);
};

// Adds a word to a given node.
void add_child(void *word_arg, BKNode *self) {
  uint8_t *word = word_arg;
  self->word = word;
  self->empty = 0;
};

// Returns an initialized BK Node
BKNode init_bknode() {
  BKNode node;
  node.empty = 1;
  node.word = NULL;
  node.size = CHILD_NODES;
  node.child = handle_calloc(CHILD_NODES, sizeof(BKNode));

  // Set each child node to "empty."
  for (uint64_t i = 0; i < CHILD_NODES; i++) {
    node.child[i].empty = 1;
    node.child[i].word = NULL;
  }

  node.AddChild = add_child;

  return node;
};

void clear_children(BKNode node) {
  for (uint64_t i = 0; i < CHILD_NODES; i++) {
    if (!node.child[i].empty) {
      clear_children(node.child[i]);
      free(node.child[i].child);
      node.child[i].child = NULL;
    }
  }
}

void clear_bknode(BKNode node) {
  clear_children(node);
  free(node.child);
  node.child = NULL;
}
#endif
