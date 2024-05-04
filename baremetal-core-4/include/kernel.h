/* kernel.h */




 
//typedef unsigned long uintptr_t;

/* Function declaration return types */

#define local	static		/* Local procedure or variable declar.	*/

/* Universal return constants */
//#define NULL	(void *)0		/* null pointer for linked lists	*/
#define NULLCH	'\0'		/* null character			*/
#define	NULLSTR	""		/* null string				*/

#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	EOF	(-2)		/* End-of-file (usually from read)	*/
#define	TIMEOUT	(-3)		/* system call timed out		*/

 
#define	MINSTK	400		/* minimum stack size in bytes		*/

#define	CONTEXT	64		/* bytes in a function call context on	*/
				/* the run-time stack			*/
#define	QUANTUM	10		/* time slice in milliseconds		*/

/* Size of the stack for the null process */

#define	NULLSTK		4096	/* stack size for null process		*/

/* Prototypes of I/O functions used throughout the kernel */

//uint32	kprintf(const char *fmt, ...);
//uint32	kputc(byte);
//uint32	kgetc(void);
#define PEND_SV()		*((unsigned int *)0xE000ED04) = (1 << 28);



#define _BMD(reg, msk, val)     (reg) = (((reg) & ~(msk)) | (val))
/* set bitfield */
#define _BST(reg, bits)         (reg) = ((reg) | (bits))
/* clear bitfield */
#define _BCL(reg, bits)         (reg) = ((reg) & ~(bits))
/* wait until bitfield set */
#define _WBS(reg, bits)         while(((reg) & (bits)) == 0)
/* wait until bitfield clear */
#define _WBC(reg, bits)         while(((reg) & (bits)) != 0)
/* wait for bitfield value */
#define _WVL(reg, msk, val)     while(((reg) & (msk)) != (val))
/* bit value */
#define _BV(bit)                (0x01 << (bit))


extern int ready_preemptive;




