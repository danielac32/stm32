/* OS system calls */

enum {
	OS_NULLPROCESS = 0,
	OS_UPTIME,
	OS_PUTC,
	OS_CLOSE, /* Devices */
	OS_CONTROL,
	OS_GETC,
	OS_INIT,
	OS_OPEN,
	OS_READ,
	OS_SEEK,
	OS_WRITE,

	OS_CREATE, /* Processes */ 
	OS_CHPRIO,
	OS_KILL,	/* Exit */
	OS_GETPRIO,
	OS_READY,
	OS_RECEIVE,
	OS_RECVCLR,
	OS_RECVTIME,
	OS_RESCHED_CNTL,
	OS_RESUME,
	OS_SEND,
	OS_SIGNAL,
	OS_SIGNALN,
	OS_SLEEP,
	OS_SLEEPMS,
	OS_SUSPEND,
	OS_UNSLEEP,
	OS_WAKE,
	OS_WAKEUP, 
	OS_YIELD,
	
	OS_MKBUFPOOL, /* Buffers */
	OS_FREEBUF,
	OS_FREEMEM,
	OS_GETBUF,
	
	OS_KPRINTF, /* Kernel */
	OS_KPUTC,
	OS_GETDEV,
	OS_GETMEM,
	OS_GETSTK,
	
	OS_PTCOUNT, /* Ports */
	OS_PTCREATE,
	OS_PTDELETE,
	OS_PTINIT,
	OS_PTRCV,
	OS_PTRESET,
	
	OS_SEMCOUNT, /* Semaphores */
	OS_SEMCREATE,
	OS_SEMRESET,
	OS_SEMDELET,

};

extern syscall init(did32     descrp);
extern syscall	write(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	count		/* Length of buffer		*/
	);
extern syscall	read(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	count		/* Length of buffer		*/
	);
extern syscall	open(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*name,		/* Name to use, if any		*/
	  char		*mode		/* Mode for device, if any	*/
	);