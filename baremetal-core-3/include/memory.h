/* memory.h - roundmb, truncmb, freestk */
#include "queue.h"
#define MAXADDR		0x20020000		/* 128kB SRAM */
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


#define ALIGN(to, x)  ((x+(to)-1) & ~((to)-1))


#define KMALLOC_START  (void*)&end
#define KMALLOC_LENGTH ((MAXADDR) - (size_t)(&end))-1024





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



extern sem_t sem_malloc;

