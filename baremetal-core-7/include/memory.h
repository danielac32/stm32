#include <stdint.h>
#include "queue.h"


#define MAXADDR		0x2001f400		/* 128kB SRAM */
#define HANDLERSTACK	1024			/* Size reserved for stack in Handler mode */

extern	unsigned int	text;			/* Start of text segment	*/
extern	unsigned int	etext;			/* End of text segment		*/
extern	unsigned int	data;			/* Start of data segment	*/
extern	unsigned int	edata;			/* End of data segment		*/
extern	unsigned int	bss;			/* Start of bss segment		*/
extern	unsigned int	ebss;			/* End of bss segment		*/
extern	unsigned int	end;			/* End of program		*/

#define KMALLOC_START  (void*)&end

#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/

void	meminit(void);
uint32_t heap_free(void);
extern sem_t sem_malloc;