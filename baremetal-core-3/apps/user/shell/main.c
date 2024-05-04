
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include <kernel.h>
#include <elf.h>


int gets_base2(char *buffer)
{
	int buffer_index = 0;
	int next_char = 0;
	
	while (next_char != '\r' && next_char != '\n') {
		//syscall_sema_wait(&uart_rx);
		next_char = syscall_getc();
		//syscall_sema_signal(&uart_rx);
		if (next_char == 0)
			continue;

		if (next_char == '\r' || next_char == '\n')
			break;

		if (next_char == KEY_BACKSPACE) {
			if (buffer_index != 0) {
				buffer_index--;
			}
		}
		else if (next_char >= 32 && next_char < 128) {

			buffer[buffer_index] = next_char;
			buffer_index++;
		}
	}
	buffer[buffer_index] = 0;
    
	return buffer_index;
}


/*
const	struct	cmdent	cmdtab[] = {
	{"elf",	NULL},
	{"bin",	NULL},
	{"load",	NULL},
	{"reboot",	NULL},
	//{"devdump",	xsh_devdump},
	{"echo",	NULL},
	{"exit",	NULL},
	{"help",	NULL},
	{"kill",	NULL},
	//{"memdump",	xsh_memdump},
	{"ready",   NULL},
	{"memstat",	NULL}, 
	{"ps",		NULL},
	{"test",	NULL},
	{"dump",	NULL},

};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);
*/




int main(int argc, char const *argv[])
{
	char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
				SHELL_MAXTOK];  /* Contiguous null-terminated	*/
	int32	tlen;			/* Current length of all data	*/
						/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	//int	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
		
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
		
	//char	*src, *cmp;		/* Pointers used during name	*/
						/*   comparison			*/
	//bool8	diff;			/* Was difference found during	*/
	char	*args[SHELL_MAXTOK];

	exec_img ximg;
	int (*p)(int,char **);
	void *fptr;

    while(1){
    	printf(SHELL_PROMPT);
		len = gets_base2(buf);
		if (len == EOF) {
			break;
		}
		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;
		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);
        if (ntok == SYSERR) {
			printf("%s\n", SHELL_SYNERRMSG);
			continue;
		}
        if (ntok == 0) {
			printf("\n");
			continue;
		}

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			printf(SHELL_SYNERRMSG);
			continue;
		}


		for (i=0; i<ntok; i++) {
			args[i] = &tokbuf[tok[i]];
			//printf("%s\n", args[i]);
		}

//		printf("buscar elf\n");

		//printf("0x%08x\n", ximg.start);
        //int ret = syscall_elf_execve(args[0],&ximg);
        int ret = elf_execve(args[0],&ximg);
        ///printf("%s %d %s\n", args[0],ret,backgnd?"true":"false");
        //printf("0x%08x\n", ximg.start);

        if(ret == -1){
        	printf("command %s not found\n", tokbuf);
			continue;
        }


		if (backgnd == FALSE){
        	p= (int *)ret;
            int r=p(ntok,&args[0]);
            //printf("exit task %d\n",r );
            syscall_free(ximg.start);	
        }else{
        	p = (int *)ret;
		    int child = syscall_create(p,STACK_SIZE, 1,args[0]);
			task_t *prptr = (task_t *)syscall_struct_task(child);
			prptr->elf = TRUE;
			prptr->img = (void *)ximg.start;
			strcpy(prptr->name,args[0]);
			context_t *ctx = (context_t*)prptr->sp;
			ctx->r0 = ntok;
			ctx->r1 = &args[0];
			syscall_ready(child);
			syscall_sleep(10);
			continue;
        }
	}
	return 0;
}