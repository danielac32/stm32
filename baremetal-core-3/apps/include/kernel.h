/* kernel.h */

/* General type declarations used throughout the kernel */



typedef unsigned int	UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char	BYTE;	/* char must be 8-bit */
typedef unsigned short	WORD;	/* 16-bit unsigned integer */
typedef unsigned short	WCHAR;	/* 16-bit unsigned integer */
typedef unsigned long	DWORD;	/* 32-bit unsigned integer */
typedef unsigned long 	dword;
typedef unsigned short	word;	/* 16-bit unsigned integer */
typedef unsigned int 	size_t;
typedef unsigned int 	uint;
typedef unsigned short	ushort;


typedef unsigned char          uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long int uint64_t;
//typedef unsigned long uintptr_t;


typedef	unsigned char	byte;
typedef	unsigned char	uint8;
typedef	int		int32;
typedef	short		int16;
typedef	unsigned int	uint32;
typedef	unsigned short	uint16;
typedef	unsigned long long uint64;

/* Xinu-specific types */

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

typedef int32	syscall;	/* system call declaration		*/
typedef int32	devcall;	/* device call declaration		*/
typedef int32	shellcmd;	/* shell command declaration		*/
typedef int32	process;	/* top-level function of a process	*/
typedef	void	interrupt;	/* interrupt procedure			*/
typedef	int32	status;		/* returned status value (OK/SYSERR)	*/

#define local	static		/* Local procedure or variable declar.	*/

/* Boolean constants */
#define bool   _Bool
#define true   1
#define false  0

#define FALSE	0		/* Boolean False			*/
#define TRUE	1		/* Boolean True				*/

/* Null pointer, character, and string definintions */

#define NULL	(void *)0		/* null pointer for linked lists	*/
#define NULLCH	'\0'		/* null character			*/
#define	NULLSTR	""		/* null string				*/

/* Universal return constants */

#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	EOF	(-2)		/* End-of-file (usually from read)	*/
#define	TIMEOUT	(-3)		/* system call timed out		*/


#define	MINSTK	400		/* minimum stack size in bytes		*/

#define	CONTEXT	64		/* bytes in a function call context on	*/
				/* the run-time stack			*/
#define	QUANTUM	1		/* time slice in milliseconds		*/

/* Size of the stack for the null process */

#define	NULLSTK		4096	/* stack size for null process		*/

/* Prototypes of I/O functions used throughout the kernel */

#define MAX_NUMBER_OF_TASKS 12
#define STACK_SIZE    4096/2
#define MAX_ARG 4




#define PR_FREE   0 /* Process table entry is unused  */
#define PR_CURR   1 /* Process is currently running   */
#define PR_READY  2 /* Process is on ready queue    */
#define PR_RECV   3 /* Process waiting for message    */
#define PR_SLEEP  4 /* Process is sleeping      */
#define PR_SUSP   5 /* Process is suspended     */
#define PR_WAIT   6 /* Process is on semaphore queue  */
#define PR_RECTIM 7 /* Process is receiving with timeout  */
#define PR_KILL     8