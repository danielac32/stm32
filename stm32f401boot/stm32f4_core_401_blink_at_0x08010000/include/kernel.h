/* kernel.h */


#include <stdint.h>

typedef unsigned int	UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char	BYTE;	/* char must be 8-bit */
typedef unsigned short	WORD;	/* 16-bit unsigned integer */
typedef unsigned short	WCHAR;	/* 16-bit unsigned integer */
typedef unsigned long	DWORD;	/* 32-bit unsigned integer */
typedef unsigned long 	dword;
typedef unsigned short	word;	/* 16-bit unsigned integer */
typedef BYTE	DSTATUS;
//typedef unsigned long uintptr_t;

/* Function declaration return types */

//#define local	static		/* Local procedure or variable declar.	*/

/* Universal return constants */
//#define NULL	(void *)0		/* null pointer for linked lists	*/
#define NULLCH	'\0'		/* null character			*/
#define	NULLSTR	""		/* null string				*/

#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
//#define	EOF	(-2)		/* End-of-file (usually from read)	*/
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

#define PEND_SV()	*((unsigned int *)0xE000ED04) = (1 << 28);

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

 

typedef	int32	sid32;		/* semaphore ID				*/
typedef	int16	qid16;		/* queue ID				*/
typedef	int32	pid32;		/* process ID				*/
typedef	int32	did32;		/* device ID				*/
typedef	int16	pri16;		/* process priority			*/
typedef	uint32	umsg32;		/* message passed among processes	*/
typedef	int32	bpid32;		/* buffer pool ID			*/
typedef	byte	bool8;		/* Boolean type				*/
typedef	uint32	intmask;	/* saved interrupt mask			*/
typedef	int32	ibid32;		/* index block ID (used in file system)	*/
typedef	int32	dbid32;		/* data block ID (used in file system)	*/
typedef	int32	uid32;		/* ID for UDP table descriptor		*/

/* Function declaration return types */

//typedef int32	syscall;	/* system call declaration		*/
typedef int32	devcall;	/* device call declaration		*/
typedef int32	shellcmd;	/* shell command declaration		*/
typedef int32	process;	/* top-level function of a process	*/
typedef	void	interrupt;	/* interrupt procedure			*/
//typedef	int32	status;		/* returned status value (OK/SYSERR)	*/


#define CONSOLE              0	/* type tty      */
#define NULLDEV              1	/* type null     */
#define NAMESPACE            2	/* type nam      */
#define SPI_1                 3	/* type spi      */

/* Control block sizes */

#define	Nnull	1
#define	Ntty	1
#define	Nnam	1
#define	Nspi	1

#define NDEVS 4


/* Configuration and Size Constants */

#define	NPROC	     10		/* number of user processes		*/
#define	NSEM	     10		/* number of semaphores			*/

extern int ready_preemptive;
