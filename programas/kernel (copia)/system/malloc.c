#include <os.h>

struct  memblk  memlist;    /* List of free memory blocks       */

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */

#define MAXADDR   0x20040000    /* 64kB SRAM */
#define HANDLERSTACK  1024      /* Size reserved for stack in Handler mode */


 
void  *minheap; /* Start address of heap  */
void  *maxheap; /* End address of heap    */


 void    meminit(void)
{
    struct  memblk *memptr; /* Memory block pointer */
 

    /* Initialize the minheap and maxheap variables */

    minheap = (void *)&_kernel_end;
    /* 1024 bytes is reserved for supervise mode handling */
    maxheap = (void *)MAXADDR - HANDLERSTACK;


    /* Initialize the memory list as one big block */

    memlist.mnext = (struct memblk *)minheap;
    memptr = memlist.mnext;

    memptr->mnext = (struct memblk *)NULL;
    memlist.mlength = memptr->mlength = (uint32_t)maxheap - (uint32_t)minheap;
}

 
char    freemem(
      char      *blkaddr,   /* Pointer to memory block  */
      uint32_t  nbytes      /* Size of block in bytes   */
    )
{
    struct  memblk  *next, *prev, *block;
    uint32_t    top;
    uint32_t mask=disable();
 
    if ((nbytes == 0) || ((uint32_t) blkaddr < (uint32_t) minheap)
              || ((uint32_t) blkaddr > (uint32_t) maxheap)) {
         //kprintf("error 1\n");
        restore(mask);
        return SYSERR;
    }

    nbytes = (uint32_t) roundmb(nbytes);    /* Use memblk multiples */
    block = (struct memblk *)blkaddr;

    prev = &memlist;            /* Walk along free list */
    next = memlist.mnext;
    while ((next != NULL) && (next < block)) {
        prev = next;
        next = next->mnext;
    }

    if (prev == &memlist) {     /* Compute top of previous block*/
        top = (uint32_t) NULL;
    } else {
        top = (uint32_t) prev + prev->mlength;
    }

    /* Ensure new block does not overlap previous or next blocks    */

    if (((prev != &memlist) && (uint32_t) block < top)
        || ((next != NULL)  && (uint32_t) block+nbytes>(uint32_t)next)) {
        //kprintf("error 2\n");
        restore(mask);
        return SYSERR;
    }

    memlist.mlength += nbytes;

    /* Either coalesce with previous block or add to free list */

    if (top == (uint32_t) block) {  /* Coalesce with previous block */
        prev->mlength += nbytes;
        block = prev;
    } else {            /* Link into list as new node   */
        block->mnext = next;
        block->mlength = nbytes;
        prev->mnext = block;
    }

    /* Coalesce with next block if adjacent */

    if (((uint32_t) block + block->mlength) == (uint32_t) next) {
        block->mlength += next->mlength;
        block->mnext = next->mnext;
    }
    restore(mask);
    return OK;
}


char    *getmem(
      uint32_t  nbytes      /* Size of memory requested */
    )
{
    uint32_t   mask;           /* Saved interrupt mask     */
    struct  memblk  *prev, *curr, *leftover;

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32_t) roundmb(nbytes);    /* Use memblk multiples */

    prev = &memlist;
    curr = memlist.mnext;
    while (curr != NULL) {          /* Search free list */

        if (curr->mlength == nbytes) {  /* Block is exact match */
            prev->mnext = curr->mnext;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);

        } else if (curr->mlength > nbytes) { /* Split big block */
            leftover = (struct memblk *)((uint32_t) curr +
                    nbytes);
            prev->mnext = leftover;
            leftover->mnext = curr->mnext;
            leftover->mlength = curr->mlength - nbytes;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);
        } else {            /* Move to next block   */
            prev = curr;
            curr = curr->mnext;
        }
    }
    restore(mask);
    return (char *)SYSERR;
}

void list(){
    struct  memblk  *memptr;    /* Ptr to memory block      */
    uint32_t    free_mem;       /* Total amount of free memory  */
   

    /* Initialize the system */
 
    free_mem = 0;
    for (memptr = memlist.mnext; memptr != NULL;
                        memptr = memptr->mnext) {
        free_mem += memptr->mlength;
    }
    printf("\n%d bytes of free memory.  Free list:\n", free_mem);
    for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
        printf("           [0x%08X to 0x%08X]\n",(uint32_t)memptr, ((uint32_t)memptr) + memptr->mlength - 1);
    }
    
}

/*


#define kassert(x)  if (!(x)) printf("Assertion `" #x "' failed!");
#define UNIT      4u // smallest allocation unit (4 bytes)

typedef uint32_t moff_t;
#define MHDR_SZ   ((moff_t)(sizeof (mhdr_t)))
#define FULLBIT   ((moff_t)(1u<<31))
#define SIZEBITS  ((moff_t)(FULLBIT-1))



#define mem_start  ((uintptr_t)(ALIGN(UNIT, (size_t)(&_kernel_end)))) // start of managed memory
#define mem_size   ((moff_t)(ALIGN(UNIT, (size_t)(&_stack_limit) - (size_t)(&_kernel_end)))) // size of managed memory

typedef uint32_t* mhdr_t;
mhdr_t hdr_at(const moff_t addr) { return (uint32_t*)(mem_start + addr); }

void set_full(mhdr_t h) { *h |= FULLBIT; }
void set_free(mhdr_t h) { *h &= ~FULLBIT; }
void set_size(mhdr_t h, moff_t sz) { kassert(sz <= SIZEBITS); *h &= ~SIZEBITS; *h |= sz; }
bool   is_full (const mhdr_t h) { return *h & FULLBIT;  }
moff_t get_size(const mhdr_t h) { return *h & SIZEBITS; }

void mk_free_block(const moff_t start, const moff_t end) {
    mhdr_t h = hdr_at(start);
    set_free(h); set_size(h, end - start - MHDR_SZ);
}

bool is_fragmented_at(moff_t addr) {
    const mhdr_t h = hdr_at(addr);
    return !is_full(h) &&
           addr + MHDR_SZ + get_size(h) < mem_size &&
           !is_full(hdr_at(addr + MHDR_SZ + get_size(h)));
}

// actually O(1) if everything works, as no more than 3 consecutive free blocks should occur
void defrag_at(const moff_t start) {
    moff_t curr = start;
    while (curr < mem_size && !is_full(hdr_at(curr))) {
        curr += MHDR_SZ + get_size(hdr_at(curr));
    }
    mk_free_block(start, curr);
}

void *kmalloc(size_t sz) {
    if (sz == 0) return NULL;
    moff_t size = ALIGN(UNIT, sz);

    // find the best fitting free block
    moff_t best_addr;
    moff_t best_size = 0;
    moff_t curr = 0;
    while (curr < mem_size) {
        mhdr_t h = hdr_at(curr);

        if (is_fragmented_at(curr)) defrag_at(curr);

        if (!is_full(h) && get_size(h) >= size) { // we only want free blocks that are big enough
            if (!best_size || get_size(h) < best_size) {
                best_size = get_size(h);
                best_addr = curr;
            }
        }
        curr += MHDR_SZ + get_size(h);
    }
    if (!best_size) return NULL; // no free block was big enough

    if (best_size - size <= MHDR_SZ) {
        // then we need to allocate more, because a block does not fit between this and the next one
        size = best_size;
    }

    mhdr_t h = hdr_at(best_addr); set_size(h, size); set_full(h);
    if (size != best_size) { // then there is a new free block
        mk_free_block(best_addr + MHDR_SZ + size, best_addr + MHDR_SZ + best_size);
    }
    
    return (void*)(mem_start + best_addr + MHDR_SZ);
}

void kfree(void *ptr) {
    if (ptr == NULL) return; // just ignore NULL

    const moff_t blockbeg = (size_t)ptr - mem_start - MHDR_SZ;
    mhdr_t h = hdr_at(blockbeg);
    kassert(is_full(h));
    set_free(h);
}


void mm_init(void) {
    // needed by kmalloc/kfree
    printf("\n bytes of free memory: %d\n", mem_size);
    mk_free_block(0, mem_size); // create a single free block spanning all memory
    //kmalloc(10240);


}


#include <os.h>
*/


struct kmalloc_chunk *head = 0;

void malloc_init(void *start, int length)
{
  head = (struct kmalloc_chunk *) start;
  head->state = KMALLOC_STATE_FREE;
  head->length = length;
  head->next = 0;
  head->prev = 0;

}

static void ksplit(struct kmalloc_chunk *c, int length)
{
 // uint32 mask= disable();

  struct kmalloc_chunk *n = (struct kmalloc_chunk *) ((char *) c + length);

  n->state = KMALLOC_STATE_FREE;
  n->length = c->length - length;
  n->prev = c;
  n->next = c->next;

  if(c->next)
    c->next->prev = n;

  c->next = n;
  c->length = length;
  //restore(mask);
}

 

void *malloc(int length)
{
  // round up length to a multiple of KUNIT
  int extra = length % KUNIT;
  uint32 mask= disable();

  if(extra)
    length += (KUNIT - extra);

  // then add one more unit to accommodate the chunk header
  length += KUNIT;

  struct kmalloc_chunk *c = head;

  while(1) {
    if(!c) {
      printf("kmalloc: out of memory!\n");
      restore(mask);
      return NULL;
    }
    if(c->state == KMALLOC_STATE_FREE && c->length >= length)
      break;
    c = c->next;
  }

  // split the chunk if the remainder is greater than two units
  if(length - c->length > 2 * KUNIT) {
    ksplit(c, length);
  }

  c->state = KMALLOC_STATE_USED;

  restore(mask);
  // return a pointer to the memory following the chunk header
  return (c + 1);
}

 
static void kmerge(struct kmalloc_chunk *c)
{

 // uint32 mask= disable();
  if(!c){
    // restore(mask);
     return;
  }

  if(c->state != KMALLOC_STATE_FREE){
  //    restore(mask);
    return;
  }

  if(c->next && c->next->state == KMALLOC_STATE_FREE) {
    c->length += c->next->length;
    if(c->next->next) {
      c->next->next->prev = c;
    }
    c->next = c->next->next;
  }
 // restore(mask);
}

 
void free(void *ptr)
{

  uint32 mask= disable();
  struct kmalloc_chunk *c = (struct kmalloc_chunk *) ptr;
  c--;

  if(c->state != KMALLOC_STATE_USED) {
    printf("invalid kfree(%x)\n", ptr);
    restore(mask);
    return;
  }

  c->state = KMALLOC_STATE_FREE;

  kmerge(c);
  kmerge(c->prev);
  restore(mask);
}

void malloc_debug()
{
   uint32 mask= disable();
   struct kmalloc_chunk *c;
 
  const  char ps_msg1[] = "\nstate\tptr\tprev\tnext\tlength\t";

  //printf("state ptr      prev     next     length  ptr(%x)\n",KMALLOC_START);
  printf("%s max %d kb\n",ps_msg1,KMALLOC_LENGTH/1000);

  for(c = head; c; c = c->next) {
    if(c->state == KMALLOC_STATE_FREE) {
      printf("Free");
    } else if(c->state == KMALLOC_STATE_USED) {
      printf("Used");
    } else {
      printf("kmalloc list corrupted at %x!\n", c);
      return;
    }
    //printf("  %x         %d       %x       %x       %d\n", c,c, c->prev, c->next, c->length);
    printf("\t%x ", c);
    printf("\t%d ", c);
    printf("\t%x ", c->prev);
    printf("\t%x ", c->next);
    printf("\t%d kb", c->length/1000);
    printf("\n");
  }
  restore(mask);
}