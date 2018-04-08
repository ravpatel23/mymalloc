/*
 * Authors: Matthew A. Handzy, Alex J. Topel
 * mymalloc.c
 * Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

#define malloc(x) mymalloc(x, __FILE__ , __LINE__ )
#define free(x) myfree(x, __FILE__ , __LINE__ )
#define MAX 5000

static char myblock[MAX] = {0};

// returns the next metadata pointer or NULL
meta* iternext(meta* b) {

  return b->next?((meta*)(b->data+b->datasize)):NULL;

}

// splits a block into | meta : newdata (x) // meta : unused data |
int splitfrom(meta* b, size_t x) {

  meta* n;

  if (b->datasize<x)
    return 0;

  // enough space to re-alloc the block
  if (b->datasize >= x + sizeof(meta)) {

    n=(meta*)(b->data+x);
    n->next = b->next;
    b->next = 1;
    b->used = 1;
    n->datasize = b->datasize-x-sizeof(meta);
    n->used=0;
    b->datasize = x;
    n->prevdatasize = x;

    if ((b = iternext(n)))
      b->prevdatasize = n->datasize;

    return 1;

  }

  b->used = 1;
  return 1;

}

// malloc
void * mymalloc(size_t x, char * file, int line) {

  // cannot allocate 0 bytes
  if (x == 0)
    return NULL;

  // cannot allocate more than 5000 - sizeof(meta) bytes
  if (x > MAX - sizeof(meta)) {
    printf("Error allocating %zu bytes. There is not enough space.\n", x);
    return NULL;
  }

  // block pointer to navigate
  meta* block = (meta*)myblock;

  // never initialized case
  if (!block->used && !block->datasize) {
    block->datasize = sizeof(myblock)-sizeof(meta);
  }

  do {
    if (!block->used && splitfrom(block,x)){
      return block->data;
    }
  } while ((block = iternext(block)));

  return NULL;

}

// freeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
void myfree(void * x, char * file, int line) {

  // check if block exists
  // free block

  meta* b = (meta*)(((char*)x) - sizeof(meta)), *c=NULL, *d=NULL;
  char cprev = 0, cnext = 0;

  // freeing a pointer that wasn't malloced
  if ((char*)b < myblock || (char*)b >= myblock+sizeof(myblock)){
    printf("Error: pointer to free (%p) from file: %s from line: %d resulted in an error.\n", x, file, line);
    return;
  }

  // freeing an already free pointer
  if (!b->used) {
    printf("Error: pointer to free (%p) from file: %s from line: %d resulted in an error.\n", x, file, line);
    return;
  }

  // checks for prev/next free blocks for consolidation
  if (b->prevdatasize) {
    c = (meta*)(((char*)b) - b->prevdatasize - sizeof(meta));

    if (!c->used)
      cprev=1;
  }
  if (b->next) {
    d = (meta*)(b->data + b->datasize);

    if(!d->used)
      cnext=1;
  }

  // frees the pointer and consolidates previous or next free blocks
  if (cprev && cnext) {
    c->datasize += b->datasize + d->datasize + 2*sizeof(meta);
    if ((c->next = d->next)){
      d = (meta*)(d->data + d->datasize);
      d->prevdatasize = c->datasize;
    }
  } else if (cprev){
    c->datasize += b->datasize+sizeof(meta);
    if ((c->next = b->next)) {
      d->prevdatasize = c->datasize;
    }
  } else if (cnext) {
    b->used = 0;
    b->datasize += d->datasize+sizeof(meta);
    if ((b->next = d->next)) {
      c = (meta*)(d->data+d->datasize);
      c->prevdatasize = b->datasize;
    }
  } else{
    b->used=0;
  }

}
