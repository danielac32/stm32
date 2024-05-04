
#include <xinu.h>
#include <kernel.h>


#if MALLOC_MODE == 1

struct kmalloc_chunk *head = 0;

void malloc_init(void *start, int length)
{
  head = (struct kmalloc_chunk *) start;
  head->state = KMALLOC_STATE_FREE;
  head->length = length;
  head->next = 0;
  head->prev = 0;
  //head->pid = 0;
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

 

void *malloc(uint32 length)
{
  // round up length to a multiple of KUNIT
  uint32 mask= disable();
  sem_wait(&sem_malloc);
  int extra = length % KUNIT;
  //

  if(extra)
    length += (KUNIT - extra);

  // then add one more unit to accommodate the chunk header
  length += KUNIT;

  struct kmalloc_chunk *c = head;

  while(1) {
    if(!c) {
      kprintf("kmalloc: out of memory!\n");
      //restore(mask);
      sem_signal(&sem_malloc);
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

  //restore(mask);
  // return a pointer to the memory following the chunk header
  //c->pid=getpid();
  sem_signal(&sem_malloc);
  restore(mask);
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
  sem_wait(&sem_malloc);
  //uint32 mask= disable();
  struct kmalloc_chunk *c = (struct kmalloc_chunk *) ptr;
  c--;

  if(c->state != KMALLOC_STATE_USED) {
    kprintf("invalid kfree(%x)\n", ptr);
    //restore(mask);
    sem_signal(&sem_malloc);
    restore(mask);
    return;
  }

  c->state = KMALLOC_STATE_FREE;

  kmerge(c);
  kmerge(c->prev);
  //restore(mask);
  //c->pid=getpid();
  sem_signal(&sem_malloc);
  restore(mask);
}

void malloc_debug()
{

   //sem_wait(&sem_malloc);
   uint32 mask= disable();
   struct kmalloc_chunk *c;
  //kprintf("state ptr      prev     next     length  ptr(%x)\n",KMALLOC_START);

  for(c = head; c; c = c->next) {
    if(c->state == KMALLOC_STATE_FREE) {
      kprintf("Free");
    } else if(c->state == KMALLOC_STATE_USED) {
      kprintf("Used");
    } else {
      kprintf("kmalloc list corrupted at %x!\n", c);
      return;
    }
    //kprintf("  %x         %d       %x       %x       %d\n", c,c, c->prev, c->next, c->length);
    kprintf("\t%x ", c);
    kprintf("\t%d bytes", c->length);
    kprintf("\n");
  }
  restore(mask);
}

  
void testmem(){
char *a[11];   
for (int i = 0; i < 5; ++i)
{
 a[i]=malloc(10000);
}

malloc_debug();


for (int i = 0; i < 5; ++i)
{
    free(a[i]);
}

malloc_debug();
}



void    *realloc(void *ptr, size_t len)
{
    void    *real;

    real = malloc(len);
    memset(real, 0, len);
    if (real)
        memcpy(real, ptr, len);
    free(ptr);
    return (real);
}


#elif MALLOC_MODE == 2
struct  memblk  memlist;
void  *minheap; /* Start address of heap  */
void  *maxheap; /* End address of heap    */
void testmem(){
  struct  memblk  *memptr;   
  uint32  free_mem; 
  free_mem = 0;
  for (memptr = memlist.mnext; memptr != NULL;
            memptr = memptr->mnext) {
    free_mem += memptr->mlength;
  }
  printf("%10d bytes of free memory.\n", free_mem);
}

void  meminit(void)
{
  struct  memblk *memptr; /* Memory block pointer */

  /* Initialize the minheap and maxheap variables */

  minheap = (void *)&end;
  /* 1024 bytes is reserved for supervise mode handling */
  maxheap = (void *)MAXADDR - HANDLERSTACK;

  /* Initialize the memory list as one big block */

  memlist.mnext = (struct memblk *)minheap;
  memptr = memlist.mnext;

  memptr->mnext = (struct memblk *)NULL;
  memlist.mlength = memptr->mlength =
    (uint32)maxheap - (uint32)minheap;
}


uint32 freemem(char    *blkaddr,uint32  nbytes){
  uint32 mask;     /* Saved interrupt mask   */
  struct  memblk  *next, *prev, *block;
  uint32  top;

  mask = disable();
  if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
        || ((uint32) blkaddr > (uint32) maxheap)) {
    restore(mask);
    return SYSERR;
  }

  nbytes = (uint32) roundmb(nbytes);  /* Use memblk multiples */
  block = (struct memblk *)blkaddr;

  prev = &memlist;      /* Walk along free list */
  next = memlist.mnext;
  while ((next != NULL) && (next < block)) {
    prev = next;
    next = next->mnext;
  }

  if (prev == &memlist) {   /* Compute top of previous block*/
    top = (uint32) NULL;
  } else {
    top = (uint32) prev + prev->mlength;
  }

  /* Ensure new block does not overlap previous or next blocks  */

  if (((prev != &memlist) && (uint32) block < top)
      || ((next != NULL)  && (uint32) block+nbytes>(uint32)next)) {
    restore(mask);
    return SYSERR;
  }

  memlist.mlength += nbytes;

  /* Either coalesce with previous block or add to free list */

  if (top == (uint32) block) {  /* Coalesce with previous block */
    prev->mlength += nbytes;
    block = prev;
  } else {      /* Link into list as new node */
    block->mnext = next;
    block->mlength = nbytes;
    prev->mnext = block;
  }

  /* Coalesce with next block if adjacent */

  if (((uint32) block + block->mlength) == (uint32) next) {
    block->mlength += next->mlength;
    block->mnext = next->mnext;
  }
  restore(mask);
  return OK;
}

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char    *getmem(uint32  nbytes){
  uint32 mask;     /* Saved interrupt mask   */
  struct  memblk  *prev, *curr, *leftover;

  mask = disable();
  if (nbytes == 0) {
    restore(mask);
    return (char *)SYSERR;
  }

  nbytes = (uint32) roundmb(nbytes);  /* Use memblk multiples */

  prev = &memlist;
  curr = memlist.mnext;
  while (curr != NULL) {      /* Search free list */

    if (curr->mlength == nbytes) {  /* Block is exact match */
      prev->mnext = curr->mnext;
      memlist.mlength -= nbytes;
      restore(mask);
      return (char *)(curr);

    } else if (curr->mlength > nbytes) { /* Split big block */
      leftover = (struct memblk *)((uint32) curr +
          nbytes);
      prev->mnext = leftover;
      leftover->mnext = curr->mnext;
      leftover->mlength = curr->mlength - nbytes;
      memlist.mlength -= nbytes;
      restore(mask);
      return (char *)(curr);
    } else {      /* Move to next block */
      prev = curr;
      curr = curr->mnext;
    }
  }
  restore(mask);
  return (char *)SYSERR;
}


void *malloc(uint32 nbytes)
{
    #if 1
    sem_wait(&sem_malloc);
    struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == nbytes)
    {
        sem_signal(&sem_malloc);
        return NULL;
    }

    /* make room for accounting info */
    nbytes += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(nbytes);
    if (SYSERR == (uint32)pmem)
    {
        sem_signal(&sem_malloc);
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = nbytes;
    sem_signal(&sem_malloc);
    return (void *)(pmem + 1);  /* +1 to skip accounting info */
    #endif
}

void free(void *pmem)
{
    #if 1
    sem_wait(&sem_malloc);
    struct memblk *block;

    /* block points at the memblock we want to free */
    block = (struct memblk *)pmem;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {
        sem_signal(&sem_malloc);
        return;
    }
    
    sem_signal(&sem_malloc);
    freemem((char *)block, block->mlength);
    #endif

}
#elif MALLOC_MODE == 3
//chunk_t *it=(chunk_t *)&end;

void* malloc (uint32 len) {

    sem_wait(&sem_malloc);
    chunk_t* it = (chunk_t *)&end;
    chunk_t *ch;

    if (!len) {
        return (NULL);
    }
    for (; it; it = it->next) {
        if (!it->free) { /* occupied */
            continue;
        }
        if (len + sizeof(chunk_t) > it->size) {
            continue; /* free but too small  */
        }
        if (len + sizeof(chunk_t) + sizeof(chunk_t) >= it->size) {
            /* free and just perfect in size, reserve it! */
            it->free = 0;
            //it->pid=getpid();
            sem_signal(&sem_malloc);
            return ((char*)it + sizeof(chunk_t));
        }
        /* free but big, split it! */
        ch = (chunk_t*)((char*)it + len + sizeof(chunk_t));
        ch->next = it->next;
        it->next = ch;

        ch->free = it->free;

        ch->size = it->size - (len + sizeof(chunk_t));
        it->size -= ch->size;

        it->free = 0;
        //it->pid=getpid();
        sem_signal(&sem_malloc);
        return ((char*)it + sizeof(chunk_t));
    }
    /* No more free memory */
    sem_signal(&sem_malloc);
    return (NULL);
}


void free (void *p) {
    sem_wait(&sem_malloc);
    chunk_t* it = (chunk_t *)&end;
    if (!p) {
        return;
    }
    memset(p, 0, sizeof(p));
    /* mark chunk as free */
    ((chunk_t*)((char*)p - sizeof(chunk_t)))->free = 1;

    /* merge free chunks */
    for (; it; it = it->next) {
        while (it->free && it->next && it->next->free) {
            /* merge with next free */
            it->size += it->next->size;
            it->next = it->next->next;
        }
    }
    //it->free = 1;
    //it->pid=getpid();
    sem_signal(&sem_malloc);
    return;
}


void chunklist_init (size_t heap_size) {
    chunk_t* it = (chunk_t *)&end;
    it->free = 1;
    it->size = heap_size;
    it->next = NULL;
   // it->pid=-1;
}

void info() {
    chunk_t* it = (chunk_t *)&end;
    chunk_t* ch = it;

    kprintf("============================\n");
    for (; ch; ch = ch->next) {
        kprintf("chunk:%d, size:%d, %s\n",(char*)ch - (char*)it,ch->size,ch->free ? "FREE" : "RESERVED");
    }
    kprintf("============================\n\n");
}


#elif  MALLOC_MODE == 4

#define HEAP_ALIGN 4

// Linked list of free'd memory
static alloc_t *free_blocks;
static void *heap_end;

void heap_init(void *buf)
{
  heap_end = buf;
  free_blocks = 0;
}

uint32 heap_free(void)
{
  uint32 total = 0;
    // Count free'd block sizes
  for (alloc_t *node = free_blocks; node != 0; node = node->next)
    total += node->size;
    // Add remaining free memory to that total
  return total + (0x2001f400 - (uint32)heap_end);
}

void *malloc(uint32 size)
{

  uint32 mask= disable();
  //sem_wait(&sem_malloc);
  if (size == 0){
      restore(mask);
      return 0;
  }

    // Round size to an aligned value
  size = (size + sizeof(alloc_t) + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);

    // Begin searching through free'd blocks to see if we can reuse some memory.
  alloc_t *node = free_blocks;
  alloc_t *prev = 0;
  while (node != 0) {
    if (node->size >= size) {
      // If we can use this chunk, remove it from the free_blocks chain
      if (prev != 0)
        prev->next = node->next;
      else
        free_blocks = node->next;
      node->next = 0;

      // If this chunk is really big, give back the extra space
      if (node->size > size + 64) { // TODO why 64..?
        alloc_t *leftover = (alloc_t *)((uint32)node
          + sizeof(alloc_t) + size);
        leftover->size = node->size - size - sizeof(alloc_t);
        leftover->next = 0;
        free((uint8 *)leftover + sizeof(alloc_t));

        node->size = size;
        //sem_signal(&sem_malloc);
        restore(mask);
        return (void *)((uint8 *)node + sizeof(alloc_t));
      }
      //sem_signal(&sem_malloc);
      restore(mask);
      return (void *)((uint8 *)node + sizeof(alloc_t));
    }

    prev = node;
    node = node->next;
  }

    // No reusable chunks, take from the end of the heap
  node = (alloc_t *)heap_end;
  node->size = size;
  node->next = 0;

  heap_end = (void *)((uint8 *)heap_end + size);
  //sem_signal(&sem_malloc);
  restore(mask);
  return (void *)((uint8 *)node + sizeof(alloc_t)); 
}

void *calloc(uint32 count, uint32 size)
{

  uint32 mask= disable();
  //sem_wait(&sem_malloc);
    // Simply malloc and zero
  uint8 *buf = malloc(count * size);
  for (uint32 i = 0; i < count * size; i++)
    buf[i] = 0; // TODO safe?

  //sem_signal(&sem_malloc);
  restore(mask);
  return buf;
}

void free(void *buf)
{
  uint32 mask= disable();
  //sem_wait(&sem_malloc);
  if (buf == NULL ){
     //sem_signal(&sem_malloc);
     restore(mask);
     return;
  }

    // Get the alloc_t structure of this chunk
  alloc_t *alloc = (alloc_t *)((uint8 *)buf - sizeof(alloc_t));
  if (alloc->next != 0){
     //sem_signal(&sem_malloc);
     restore(mask);
     return;
  }

    // Search through the free_blocks list to see if this free chunk can merge
    // into an adjacent free chunk.
  int merged = 0;
  for (alloc_t *prev = 0, *node = free_blocks; node != 0; prev = node, node = node->next) {
        // If the node after the current one is ours
    if ((uint32)node + sizeof(alloc_t) + node->size == (uint32)alloc) {
            // Merge by adding our node's size to this one
      merged |= 1;
      node->size += sizeof(alloc_t) + alloc->size;
      break;
    }
        // Or, if this current node is the one after ours
        else if ((uint32)buf + alloc->size == (uint32)node) {
            // Merge the current node into ours
      merged |= 1;
      alloc->size += sizeof(alloc_t) + node->size;
      alloc->next = node->next;

            // Take the current node's place in the free_blocks chain
      if (prev != 0)
        prev->next = alloc;
      else
        free_blocks = alloc;
      break;
    }
  }

    // If we couldn't merge, simply append to the chain
  if (merged == 0) {
    alloc->next = free_blocks;
    free_blocks = alloc;
  }
  //sem_signal(&sem_malloc);
  restore(mask);
}


#endif