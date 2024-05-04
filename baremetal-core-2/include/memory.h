/* memory.h - roundmb, truncmb, freestk */
#include "queue.h"
#define MAXADDR		0x2001f400		/* 128kB SRAM */
#define HANDLERSTACK	1024			/* Size reserved for stack in Handler mode */


/* Added by linker */

extern	unsigned int	text;			/* Start of text segment	*/
extern	unsigned int	etext;			/* End of text segment		*/
extern	unsigned int	data;			/* Start of data segment	*/
extern	unsigned int	edata;			/* End of data segment		*/
extern	unsigned int	bss;			/* Start of bss segment		*/
extern	unsigned int	ebss;			/* End of bss segment		*/
extern	unsigned int	end;			/* End of program		*/


#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )


struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/



#define UNIT      4u // smallest allocation unit (4 bytes)
typedef uint32 moff_t;
#define ALIGN(to, x)  ((x+(to)-1) & ~((to)-1))


#define KMALLOC_START  (void*)&end
#define KMALLOC_LENGTH ((MAXADDR) - (size_t)(&end))-4096





void malloc_init(void *start, int length);
void malloc_debug();
int malloc_test();

#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	//char pid;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

extern struct kmalloc_chunk *head;

typedef struct chunk_s {
    struct chunk_s  *next;
    size_t          size;
    char            free;
    //char 			pid;
} chunk_t;


extern chunk_t *it;



/*
void* do_malloc (chunk_t* it, size_t len);
void do_free (chunk_t* it, void *p);*/
void chunklist_init (uint32 heap_size);
void info();

extern sem_t sem_malloc;


