/* stdio.h - definintions and constants for standard I/O functions */


/* Definintion of standard input/ouput/error used with shell commands */

#define	stdin	0//((proctab[currpid]).prdesc[0])
#define	stdout	0//((proctab[currpid]).prdesc[1])
#define	stderr	0//((proctab[currpid]).prdesc[2])


/* Prototypes for formatted output functions */

extern	int	_printf(const char *, ...);
extern	int	sprintf(char *, char *, ...);
#define printf _printf
/* Prototypes for character input and output functions */