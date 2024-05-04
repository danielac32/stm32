/**
 * @file malloc.c
 * @provides malloc.c.
 * This file is deprecated in favor of mem/malloc.c and the user heap
 * allocator.  However, it remains here for backup purposes.
 *
 * $Id: malloc.c 2065 2009-09-04 21:44:36Z brylow $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>
#include <kernel.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 * @param nbytes number of bytes requested
 * @return pointer to region on success, NULL on failure
 */
void *malloc(uint32 nbytes)
{
    struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == nbytes)
    {
        return NULL;
    }

    /* make room for accounting info */
    nbytes += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(nbytes);
    if (SYSERR == (uint32)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = nbytes;

    return (void *)(pmem + 1);  /* +1 to skip accounting info */
}

void free(void *pmem)
{
    struct memblk *block;

    /* block points at the memblock we want to free */
    block = (struct memblk *)pmem;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {
        return;
    }

    freemem((char *)block, block->mlength);
}



void *aligned_malloc(size_t alignment,size_t bytes)
{
void *p1 ,*p2; // basic pointer needed for computation.

/* We need to use malloc provided by C. First we need to allocate memory
of size bytes + alignment + sizeof(size_t) . We need 'bytes' because 
user requested it. We need to add 'alignment' because malloc can give us 
any address and we need to find multiple of 'alignment', so at maximum multiple
of alignment will be 'alignment' bytes away from any location. We need 
'sizeof(size_t)' for implementing 'aligned_free', since we are returning modified 
memory pointer, not given by malloc ,to the user, we must free the memory 
allocated by malloc not anything else. So I am storing address given by malloc just above 
pointer returning to user. Thats why I need extra space to store that address. 
Then I am checking for error returned by malloc, if it returns NULL then 
aligned_malloc will fail and return NULL.
*/
if((p1 =(void *) malloc(bytes + alignment + sizeof(size_t)))==NULL)
return NULL;

/*  Next step is to find aligned memory address multiples of alignment.
By using basic formule I am finding next address after p1 which is 
multiple of alignment.I am storing new address in p2.
*/
size_t addr=(size_t)p1+alignment+sizeof(size_t);

p2=(void *)(addr - (addr%alignment));

/*  Final step, I am storing the address returned by malloc 'p1' just "size_t"
bytes above p2, which will be useful while calling aligned_free.
*/
*((size_t *)p2-1)=(size_t)p1;

return p2;
}

/************************************************
Name     :- aligned_free
Arguments   :- pointer to be freed
Returns  :- Nothing 
*************************************************/

void aligned_free(void *p )
{
free((void *)(*((size_t *) p-1)));
}