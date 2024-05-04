/* stdio.h - definintions and constants for standard I/O functions */


/* Prototypes for formatted input functions */

extern	int32	_doscan(char *,int32 *, int32 (*)(void),
			int32 (*)(char), int32, int32);
extern	int32	sscanf(char *, char *, int32);
extern	int32	fscanf(int32, char *, int32);
#define	scanf(fmt, args)	fscanf(CONSOLE, fmt, args)


/* Definintion of standard input/ouput/error used with shell commands */

#define	stdin	((proctab[currpid]).prdesc[0])
#define	stdout	((proctab[currpid]).prdesc[1])
#define	stderr	((proctab[currpid]).prdesc[2])


/* Prototypes for formatted output functions */

extern	int32	fprintf(int, char *, ...);
extern	int32	printf(const char *, ...);
extern	int32	sprintf(char *, char *, ...);


/* Prototypes for character input and output functions */

extern	int32	fgetc(int);
extern	char	*fgets(char *, int32, int32);
extern	int32	fputc(int32, int32);
extern	int32	fputs(char *, int32);
extern	int32	putchar(int32 c);
extern	int32	getchar(void);

extern void    hexdump(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     bool8  canon           /* Print in ASCII or hex    */
    );
 

extern void    hexdump2(
     void   *buffer,        /* Addresss of memory area  */
     uint32 length,         /* Length in bytes      */
     uint32 offset,
     bool8  canon           /* Print in ASCII or hex    */
    );