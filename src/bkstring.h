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

extern void bk_add(void *word_arg, BKTree *self);
extern void *search(void *word_arg, uint64_t dist, BKTree *b);
extern BKTree init_bktree();
extern void init(BKTree *b);
extern void clear_bktree(BKTree *bk);
extern void close();

#endif
