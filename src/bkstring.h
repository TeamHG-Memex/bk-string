#ifndef BK_H
#define BK_H

#include <stdint.h>
#include "bknode.h"

typedef struct BKTree BKTree;
struct BKTree {
  BKNode _root;
  void (*Add) (void *, BKTree *);
  void* (*Search) (void *, uint64_t, BKTree*);
};

BKTree init_bktree();
void clear_bktree(BKTree *bk);

#endif
