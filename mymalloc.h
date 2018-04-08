/*
 * Authors: Matthew A. Handzy, Alex J. Topel
 * mymalloc.h
 * Assignment 1
 */

#ifndef mymalloc_h_
#define mymalloc_h_

#define malloc(x) mymalloc(x, __FILE__ , __LINE__ )
#define free(x) myfree(x, __FILE__ , __LINE__ )

typedef struct _meta meta;
struct _meta {
  unsigned char used : 1;
  unsigned char next : 1;
  unsigned short datasize : 14;
  unsigned short prevdatasize;
  char data[0];
};

#endif
