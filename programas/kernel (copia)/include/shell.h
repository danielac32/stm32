#include <kernel.h>
#ifndef SHELL_H
#define SHELL_H

#define SHELL_BUFLEN  32+1  /* Length of input buffer */
#define SHELL_MAXTOK  8   /* Maximum tokens per line  */
#define SHELL_CMDSTK  256   /* Size of stack for process  */
          /*    that executes command */
#define SHELL_ARGLEN  (SHELL_BUFLEN+SHELL_MAXTOK) /* Argument area  */
#define SHELL_CMDPRIO 20    /* Process priority for command */

/* Message constants */

/* Shell banner (assumes VT100) */

#define SHELL_BAN0  "\033[31;1m"
#define SHELL_BAN1      "------------------------------------------"
#define SHELL_BAN2      "   __    __   _____    _   _    _    _    "
#define SHELL_BAN3      "   \\ \\  / /  |__ __|  | \\ | |  | |  | |   "
#define SHELL_BAN4      "    \\ \\/ /     | |    |  \\| |  | |  | |   "
#define SHELL_BAN5      "    / /\\ \\    _| |_   | \\   |  | |  | |   "
#define SHELL_BAN6      "   / /  \\ \\  |     |  | | \\ |  \\  --  /   "
#define SHELL_BAN7      "   --    --   -----    -   -     ----     "
#define SHELL_BAN8  "                                        "
#define SHELL_BAN9      "------------------------------------------"
#define SHELL_BAN10 "\033[0m\n"

/* Messages shell displays for user */

#define SHELL_PROMPT  "xsh $ "  /* Command prompt   */
#define SHELL_STRTMSG "Welcome to Xinu!\n"/* Welcome message    */
#define SHELL_EXITMSG "Shell closed\n"/* Shell exit message   */
#define SHELL_SYNERRMSG "Syntax error\n"/* Syntax error message   */
#define SHELL_CREATMSG  "Cannot create process\n"/* command error */
#define SHELL_INERRMSG  "Cannot open file %s for input\n" /* Input err  */
#define SHELL_OUTERRMSG "Cannot open file %s for output\n"/* Output err */
          /* Builtin cmd error message  */
#define SHELL_BGERRMSG  "Cannot redirect I/O or background a builtin\n"

/* Constants used for lexical analysis */

#define SH_NEWLINE  '\n'    /* New line character   */
#define SH_EOF    '\04'   /* Control-D is EOF   */
#define SH_AMPER  '&'   /* Ampersand character    */
#define SH_BLANK  ' '   /* Blank character    */
#define SH_TAB    '\t'    /* Tab character    */
#define SH_SQUOTE '\''    /* Single quote character */
#define SH_DQUOTE '"'   /* Double quote character */
#define SH_LESS   '<'   /* Less-than character  */
#define SH_GREATER  '>'   /* Greater-than character */

/* Token types */

#define SH_TOK_AMPER  0   /* Ampersand token    */
#define SH_TOK_LESS 1   /* Less-than token    */
#define SH_TOK_GREATER  2   /* Greater-than token   */
#define SH_TOK_OTHER  3   /* Token other than those */
          /*   listed above (e.g., an */
          /*   alphanumeric string) */

/* Shell return constants */

#define SHELL_OK   0
#define SHELL_ERROR  1
#define SHELL_EXIT  -3

//#define NULL  0   /* null pointer for linked lists  */
#define NULLCH  '\0'    /* null character     */
#define NULLSTR ""    /* null string        */

/* Universal return constants */

#define OK  ( 1)    /* normal system call return    */
#define SYSERR  (-1)    /* system call failed     */
#define EOF (-2)    /* End-of-file (usually from read)  */
#define TIMEOUT (-3)    /* system call timed out    */

int32_t  lexan (
    char    *line,    /* Input line terminated with */
          /*   NEWLINE or NULLCH    */
    int32_t   len,    /* Length of the input line,  */
          /*   including NEWLINE    */
    char    *tokbuf,  /* Buffer into which tokens are */
          /*   stored with a null   */
          /*   following each token */
    int32_t   *tlen,    /* Place to store number of */
          /*   chars in tokbuf    */
    int32_t   tok[],    /* Array of pointers to the */
          /*   start of each token  */
    int32_t   toktyp[]  /* Array that gives the type  */
          /*   of each token    */
  );


char  buf[SHELL_BUFLEN];  /* Input line (large enough for */
  int32_t len;      /* Length of line read    */
  char  tokbuf[SHELL_BUFLEN + /* Buffer to hold a set of  */
      SHELL_MAXTOK];  /* Contiguous null-terminated */
  int32_t tlen;     /* Current length of all data */
          /*   in array tokbuf    */
  int32_t tok[SHELL_MAXTOK];  /* Index of each token in */
  int32_t toktyp[SHELL_MAXTOK]; /* Type of each token in tokbuf */
  int32_t ntok;     /* Number of tokens on line */
  //pid32 child;      /* Process ID of spawned child  */
  bool backgnd;    /* Run command in background? */
  char  *outname, *inname;  /* Pointers to strings for file */
          /*   names that follow > and <  */
  //did32 stdinput, stdoutput;  /* Descriptors for redirected */
          /*   input and output   */
  int32_t i;      /* Index into array of tokens */
  int32_t j;      /* Index into array of commands */
  int32_t msg;      /* Message from receive() for */
          /*   child termination    */
  int32_t tmparg;     /* Address of this var is used  */
          /*   when first creating child  */
          /*   process, but is replaced */
  char  *src, *cmp;   /* Pointers used during name  */
          /*   comparison     */
  bool diff;     /* Was difference found during  */
  char  *args[SHELL_MAXTOK];  /* Argument vector passed to  */
          /*   builtin commands   */
  //did32 dev = 0;    /* ID of tty device from which  */






///////////////////////////////////////////////
struct  cmdent  {     /* Entry in command table */
  char  *cname;     /* Name of command    */
  bool cbuiltin;   /* Is this a builtin command? */
  int32 (*cfunc)(int32,char*[]);
};

extern const struct  cmdent  cmdtab[];
extern uint32_t  ncmd;


#endif