


#ifndef KMALLOC_H
#define KMALLOC_H

#define MAX_HEAPS 10000
typedef uint32_t Align;

union header {
  struct {
    union header *ptr;
    uint32_t size;
    uint32_t store;
  } s;
  Align x;
};

typedef union header Header;

static unsigned char heaps[MAX_HEAPS];
static unsigned char *program_break = heaps;

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */

void *kmalloc(uint32_t nbytes);
void kfree(void *ap);


#endif