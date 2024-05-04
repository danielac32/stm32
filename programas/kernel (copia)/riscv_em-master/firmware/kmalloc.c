#include <stdint.h>
#include <string.h>
#include "kmalloc.h"


static void *sbrk(uint32_t nbytes)
{
  if (program_break + nbytes >= heaps
      && program_break + nbytes < heaps + MAX_HEAPS) {
    unsigned char *previous_pb = program_break;
    program_break += nbytes;
    return (void *) previous_pb;
  }
  return (void *) -1;
}

void *kcalloc(uint32_t nbytes){
      void *p=kmalloc(nbytes);
      memset(p,0,nbytes);
      return p;
}

void *kmalloc(uint32_t nbytes)
{
  Header *p, *prevp,*n;
  uint32_t nunits;
  void *cp;
  n->s.store = nbytes;
  
  nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

  if ((prevp = freep) == NULL) {
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }

  for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) {
      if (p->s.size == nunits) {
        prevp->s.ptr = p->s.ptr;
      } else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void *)(p + 1);
    }

    if (p == freep) {
      cp = sbrk(nunits * sizeof(Header));
      if (cp == (void *) -1) {
        return NULL;
      } else {
        p = (Header *) cp;
        p->s.size = nunits;
        kfree((void *) (p + 1));
        p = freep;
      }
    }
  }
}

void kfree(void *ap)
{

  Header *bp, *p,*n;
  bp = (Header *) ap - 1;
  
  //memset(ap,0,n->s.store);

  for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  }

  if (bp + bp->s.size == p->s.ptr) {
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else {
    bp->s.ptr = p->s.ptr;
  }

  if (p + p->s.size == bp) {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else {
    p->s.ptr = bp;
  }

  freep = p;
  //ap=NULL;
}
