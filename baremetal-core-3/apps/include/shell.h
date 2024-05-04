/* shell.h - Declarations and constants used by the Xinu shell */

/* Size constants */

#define SHELL_BUFLEN	64	/* Length of input buffer	*/
#define SHELL_MAXTOK	6		/* Maximum tokens per line	*/
//#define SHELL_CMDSTK	4096	/* Size of stack for process	*/
					/*    that executes command	*/
#define	SHELL_ARGLEN	(SHELL_BUFLEN+SHELL_MAXTOK) /* Argument area	*/
//#define SHELL_CMDPRIO	20		/* Process priority for command	*/

/* Message constants */

/* Shell banner (assumes VT100) */

/* Messages shell displays for user */

#define SHELL_PROMPT	"xsh $ "	/* Command prompt		*/
//#define SHELL_STRTMSG	"Welcome to Xinu!\n"/* Welcome message		*/
//#define SHELL_EXITMSG	"Shell closed\n"/* Shell exit message		*/
#define SHELL_SYNERRMSG	"Syntax error\n"/* Syntax error message		*/
//#define SHELL_CREATMSG	"Cannot create process\n"/* command error	*/
//#define SHELL_INERRMSG	"Cannot open file %s for input\n" /* Input err	*/
//#define SHELL_OUTERRMSG	"Cannot open file %s for output\n"/* Output err	*/
					/* Builtin cmd error message	*/
//#define SHELL_BGERRMSG	"Cannot redirect I/O or background a builtin\n"

/* Constants used for lexical analysis */

#define	SH_NEWLINE	'\n'		/* New line character		*/
#define	SH_EOF		'\04'		/* Control-D is EOF		*/
#define	SH_AMPER	'&'		/* Ampersand character		*/
#define	SH_BLANK	' '		/* Blank character		*/
#define	SH_TAB		'\t'		/* Tab character		*/
#define	SH_SQUOTE	'\''		/* Single quote character	*/
#define	SH_DQUOTE	'"'		/* Double quote character	*/
#define	SH_LESS		'<'		/* Less-than character	*/
#define	SH_GREATER	'>'		/* Greater-than character	*/

/* Token types */

#define	SH_TOK_AMPER	0		/* Ampersand token		*/
#define	SH_TOK_LESS	1		/* Less-than token		*/
#define	SH_TOK_GREATER	2		/* Greater-than token		*/
#define	SH_TOK_OTHER	3		/* Token other than those	*/
					/*   listed above (e.g., an	*/
					/*   alphanumeric string)	*/

/* Shell return constants */

#define	SHELL_OK	 0
#define	SHELL_ERROR	 1
#define	SHELL_EXIT	-3

/* Structure of an entry in the table of shell commands */

struct	cmdent	{			/* Entry in command table	*/
	char	cname[16];			/* Name of command		*/
	//bool8	cbuiltin;		/* Is this a builtin command?	*/
	int	(*cfunc)(int,char*[]);/* Function for command		*/
};

extern	uint32	ncmd;
extern	const	struct	cmdent	cmdtab[];
extern int32	lexan (
	  char		*line,		/* Input line terminated with	*/
					/*   NEWLINE or NULLCH		*/
	  int32		len,		/* Length of the input line,	*/
					/*   including NEWLINE		*/
	  char		*tokbuf,	/* Buffer into which tokens are	*/
					/*   stored with a null		*/
					/*   following each token	*/
	  int32		*tlen,		/* Place to store number of	*/
					/*   chars in tokbuf		*/
	  int32		tok[],		/* Array of pointers to the	*/
					/*   start of each token	*/
	  int32		toktyp[]	/* Array that gives the type	*/
					/*   of each token		*/
	);

enum SpecialKey {
    SPECIAL_KEYS_START = 0x1000,
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_RIGHT,
    KEY_ARROW_LEFT,
    KEY_BACKSPACE
};

typedef struct {
  unsigned int r4, r5, r6, r7, r8, r9, r10, r11;
  unsigned int r0, r1, r2, r3, r12, lr, pc, psr;
} context_t;

typedef struct task_t
{
    //uint32  *prstkptr;  /* Saved stack pointer      */
    void  *prstkbase; /* Base of run time stack   */
    void  *sp;
    uint32  prstklen; /* Stack length in bytes    */
    //int pargs;              
    void *parg[MAX_ARG]; 
    //int *paddr;
    uint8 pid;
    uint8 prio;
    uint8 state;
    //uint32 wait_ticks;
    uint32 param;
    //sem_t sem;
    char name[16];
    bool8   elf;
    void *img;
    
    struct task_t *next;
}task_t;