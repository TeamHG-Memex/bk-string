#ifndef BK_NODE_H
#define BK_NODE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct BKNode BKNode;
struct BKNode {
  uint8_t *word;
  BKNode *child;
  uint64_t empty;
  uint64_t size;

  void (*AddChild) (void *, BKNode *);
};

BKNode init_bknode();
void clear_bknode(BKNode node);
#endif
