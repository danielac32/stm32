/* stdio.h - definintions and constants for standard I/O functions */



#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02

/* Definintion of standard input/ouput/error used with shell commands */
 


/* Prototypes for formatted output functions */

extern	int32	fprintf(int, char *, ...);
extern	int32	printf(const char *, ...);
extern	int32	sprintf(char *, char *, ...);

#define gets(buffer) gets_base(kgetch, buffer)


void hexDump(uint32 offset,char *desc, void *addr, int len);
void hexDump2(uint32 offset, void *addr, int len);