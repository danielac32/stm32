
#if 0
#include <xinu.h>
#include <elf.h>
#include <cache.h>
#include <queue.h>
extern sem_t sem_task[];
//static int xsh_dump(int nargs, char *args[]);
static int xsh_ready(int nargs, char *args[]);
const	struct	cmdent	cmdtab[] = {
	{"elf",	xsh_elf},
	{"bin",	xsh_bin},
	{"load",	xsh_load},
	{"reboot",	xsh_reboot},
	//{"devdump",	xsh_devdump},
	{"echo",	xsh_echo},
	{"exit",	xsh_exit},
	{"help",	xsh_help},
	{"kill",	xsh_kill},
	//{"memdump",	xsh_memdump},
	{"ready",   xsh_ready},
	{"memstat",	xsh_memstat}, 
	{"ps",		xsh_ps},
	{"test",	xsh_test},
	{"dump",	xsh_dump},

};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);


int shell(int argc, char *argv[])
{
    

#if 1
char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
int32	len;			/* Length of line read		*/
char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
int32	ntok;			/* Number of tokens on line	*/
int	child;			/* Process ID of spawned child	*/
bool8	backgnd;		/* Run command in background?	*/
	
int32	i;			/* Index into array of tokens	*/
int32	j;			/* Index into array of commands	*/
	
char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
bool8	diff;			/* Was difference found during	*/
char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/

#else
static char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	static int32	len;			/* Length of line read		*/
	static char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
    static int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	static int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	static int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	static int32	ntok;			/* Number of tokens on line	*/
	static int	child;			/* Process ID of spawned child	*/
	static bool8	backgnd;		/* Run command in background?	*/
	
	static int32	i;			/* Index into array of tokens	*/
	static int32	j;			/* Index into array of commands	*/
	
	static char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
	static bool8	diff;			/* Was difference found during	*/
	static char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/
#endif

    //syscall_listdirectory("/");
	while(1){
		printf(SHELL_PROMPT);
		//len = gets_base2(buf);
		len = gets_base2(buf);
		printf("%s\n",buf );
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

		for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
					break;
				}
				src++;
				cmp++;
			}
			if (diff || (*cmp != NULLCH)) {
				continue;
			} else {
				break;
			}
		}
		if (j >= ncmd) {
			printf("command %s not found\n", tokbuf);
			continue;
		}


        for (i=0; i<ntok; i++) {
			args[i] = &tokbuf[tok[i]];
		}

        
        if (backgnd == FALSE){
        	(*cmdtab[j].cfunc)(ntok, args);
        }else{
        	 sem_wait(&sem_task[syscall_getpid()]);
        	 //printf("\n");
        	 //child = create(cmdtab[j].cfunc,SHELL_CMDSTK, SHELL_CMDPRIO,cmdtab[j].cname,0);//
        	 child = syscall_create(cmdtab[j].cfunc,STACK_SIZE, SHELL_CMDPRIO,cmdtab[j].cname);
			 task_t *prptr = &task[child];
			 cmcm_stack_frame_t *ctx = (cmcm_stack_frame_t*)prptr->sp;
			 ctx->hw_frame.r0 = (uint32) ntok;
			 ctx->hw_frame.r1 = (uint32)&args;
	         syscall_ready(child);
	         //printf("%s %d\n",cmdtab[j].cname,child );
	         sem_signal(&sem_task[getpid()]);
	         continue;
        }
        
        /* child = syscall_create(cmdtab[j].cfunc,SHELL_CMDSTK, SHELL_CMDPRIO,cmdtab[j].cname);
		 task_t *prptr = &task[child];
		 context_t *ctx = (context_t*)prptr->sp;
		 ctx->r0 = (uint32) ntok;
		 ctx->r1 = (uint32)&args;
         syscall_ready(child);*/

		//child = create(cmdtab[j].cfunc,SHELL_CMDSTK, SHELL_CMDPRIO,cmdtab[j].cname,0);
		
       
        //printf("->%s\n", buf);
        //memset(buf,0,SHELL_BUFLEN);
	}
	return 0;
}


static void dummy(char *s){
    while(*s){
        *s--;
    }
}
FILE *file_read;

typedef struct t_cache
{
	uint32    cache_idx[32];
    uint8    cache_data[32];
}cache;
  

static int cache_get(cache *file,uint32 clusterIdx, uint8 *pNextCluster)
{
    uint32 slot = clusterIdx % 32;

    if (file->cache_idx[slot] == clusterIdx)
    {
        *pNextCluster = file->cache_data[slot];
        return 1;
    }
    return 0;
}

static int cache_set(cache *file, uint32 clusterIdx, uint8 nextCluster)
{
    uint32 slot = clusterIdx % 32;

    if (file->cache_idx[slot] == clusterIdx)
        file->cache_data[slot] = nextCluster;
    else
    {
        file->cache_idx[slot] = clusterIdx;
        file->cache_data[slot] = nextCluster;
    }
    return 1;
}


static uint8 read(cache *file,uint32 pos){
	uint8 res;


	if (!cache_get(file, pos, &res))
    {
	    syscall_fseek(file_read, pos, SEEK_SET);
	    syscall_fread(&res,1,1, file_read); 
	    // Push entry into cache
	    cache_set(file, pos, res);
    }
	//syscall_fseek(file_read, pos, SEEK_SET);
	//syscall_fread(&res,1,1, file_read); 
	return res;
}

int xsh_test(int nargs, char *args[]){
cache buf;
for (int i=0;i<32;i++)
{
    buf.cache_idx[i] = 0xFFFFFFFF; // Not used
    buf.cache_data[i] = 0;
}
if ((file_read = syscall_fopen("/a.out","r")) == NULL){
       	printf("Error! opening file");
        return -1;
}

uint8 res;

uint32 start = sys_time;
for (int i = 0; i < 32; ++i)
{
	res = read(&buf,i);
	printf("%02x\n",res );
}
uint32 end=sys_time;
printf("time: %d\n",end-start );

start = sys_time;
for (int i = 0; i < 32; ++i)
{
	res = read(&buf,i);
	printf("%02x\n",res );
}
end=sys_time;
printf("time2: %d\n",end-start );
syscall_fclose(file_read);
return 0; 
}


static int xsh_ready(int nargs, char *args[]){
	//syscall_ready(atoi(args[1]));

	/*int ret = elf_execve("/blink1.elf");
    int (*p)(int,char **);
    
    p= (int *)ret;
    p(0,0);	
	printf("%d\n", ret);*/


	return 0;
}

//t_cache cache1;

int xsh_bin(int nargs, char *args[]){

    uint64 thit, taccessed;
	init_cache(/*&cache1,*/"/cache.o");
	
    
    /*int j;
    char c=0xaa;
    for (int i = 0; i < 6000; ++i)
    {
    	cache_write(i,c, 1);
    }

    for (int i = 0; i < 6000; ++i)
    {
    	cache_read(i,&c, 1);
    	printf("%02x\n",c );
    }

    uint64 thit, taccessed;

	cache_get_stat(&thit, &taccessed);
	printf( "hit: %d accessed: %d\n", thit, taccessed);*/
   

    char c=0x33;
    for (int i = 0; i < 5000; ++i)
    {
    	 cache_write(i,c, 1);
    }
    cache_get_stat(&thit, &taccessed);
	printf( "hit: %d accessed: %d\n", thit, taccessed);
    
    
    char f;
    cache_read(0,&f, 1);
    printf("%02x\n",f );
    
    cache_get_stat(&thit, &taccessed);
	printf( "hit: %d accessed: %d\n", thit, taccessed);




	/*FILE *fptr;

    //for (int i = 0; i < nargs; ++i){
      //  dummy(args[i]);
    //}

    if ((fptr = syscall_fopen(args[1],"r")) == NULL){
       	printf("Error! opening file");
        return -1;
    }
    uint8 *buff;
    syscall_fseek(fptr, 0, SEEK_END);
    int lsize = syscall_ftell(fptr);
    printf("size: %d\n",lsize );
    buff=syscall_malloc(lsize);
    syscall_fseek(fptr, 0, SEEK_SET);
  	syscall_fread(buff,lsize,1, fptr); 
    syscall_fclose(fptr); 

    int (*p)(int,char ** )= (int *)((uintptr_t)buff | 1);
    p(0,NULL);*/
    /*int pid = syscall_getpid();
	task_t *prptr = (task_t *)syscall_struct_task(pid);
	prptr->elf = TRUE;
	prptr->img = buff;
	strcpy(prptr->name,args[1]);

    int (*p)(int,char ** )= (int *)((uintptr_t)buff | 1);
    p(0,NULL);*/

    


	return 0;
}
int xsh_dump(int nargs, char *args[]){
    FILE *fptr;

    //for (int i = 0; i < nargs; ++i){
        //dummy(args[i]);
    //}

    if ((fptr = syscall_fopen(args[1],"r")) == NULL){
       	printf("Error! opening file");
        return -1;
    }
    uint8 *buff;
    int part=0;
    syscall_fseek(fptr, 0, SEEK_END);
    int lsize = syscall_ftell(fptr);
    printf("size: %d\n",lsize );
    #define SIZEBUF 64
    buff=syscall_malloc(SIZEBUF);

    syscall_fseek(fptr, 0, SEEK_SET);
    while(!syscall_feof(fptr)){
    	 syscall_fread(buff,SIZEBUF,1, fptr);
    	 hexDump2(part, buff, SIZEBUF);
    	 memset(buff,0,SIZEBUF);
    	 part += SIZEBUF;
    }
    syscall_free(buff);
    syscall_fclose(fptr);
   // buff=syscall_malloc(lsize);
   // syscall_fseek(fptr, 0, SEEK_SET);
   //	syscall_fread(buff,lsize,1, fptr); 
   // syscall_fclose(fptr); 
    
   // hexDump(0,args[1], buff, lsize);
   // syscall_free(buff);
    return 0;
}


int xsh_echo(int nargs, char *args[]){
   return 0;
}


int xsh_exit(int argc, char *argv[]){
	int pid = syscall_getpid();
	syscall_kill(pid);
	return 0;
}
int  xsh_ps	(int argc, char *argv[]){
	 char *pstate[]	= {		/* names for process states	*/
		"free ", "curr ", "ready", "recv ", "sleep", "susp ",
		"wait "};

    
    printf("\n");


    printf("%3s %-16s %5s %10s %10s %10s\n",
		   "Pid", "Name", "State", "Stack Base",
		   "Stack Ptr", "Stack Size");
    


	for (int i = 0; i < syscall_num_task()/*MAX_NUMBER_OF_TASKS*/; i++) {
		task_t *prptr = (task_t *)syscall_struct_task(i);//&task[i];
		if (prptr->state == PR_FREE) {  
			continue;
		}
		printf("%3d %-16s %5s 0x%08X 0x%08X %8d\n",
			i, prptr->name, pstate[(int)prptr->state],
			prptr->prstkbase,(uint32)prptr->sp, prptr->prstklen);
	}

    return 0;
}



int xsh_help(int nargs, char *args[])
{  
	printf("\n");
    for (int i=0; i<ncmd; i++) {
    	printf("->%s\n", cmdtab[i].cname);
    }
	return 0;
}


int xsh_kill(int nargs, char *args[]) {

	int32	retval;			/* return value			*/
	int	pid;			/* ID of process to kill	*/
	pid = atoi(args[1]);
	//printf("kill: %d\n",pid );
	if (pid == 0) {
		printf("%s: cannot kill the null process\n",
			args[0]);
		return 1;
	}

	task_t *prptr = (task_t *)syscall_struct_task(pid);
	if(prptr->state!=PR_FREE){
       syscall_kill(pid);
	}else{
	   printf("not found task %d\n",pid);
	}
	printf("killed\n");
	return 0;
}

int xsh_memstat(int nargs, char *args[])
{


#if MALLOC_MODE == 1
   struct kmalloc_chunk *c;
   struct kmalloc_chunk *buff=(struct kmalloc_chunk*)__syscall(XINU_KMALLOC_CHUNK);
  //printf("state ptr      prev     next     length  ptr(%x)\n",KMALLOC_START);
  printf("\n");
  for(c = buff; c; c = c->next) {
    if(c->state == KMALLOC_STATE_FREE) {
      printf("Free");
    } else if(c->state == KMALLOC_STATE_USED) {
      printf("Used");
    } else {
      printf("kmalloc list corrupted at %x!\n", c);
      return;
    }
    //printf("  %x         %d       %x       %x       %d\n", c,c, c->prev, c->next, c->length);
    printf("\t%x ", c);
    printf("\t%d bytes", c->length);
    printf("\n");
  }
#elif MALLOC_MODE == 2
	testmem();
#elif MALLOC_MODE == 3
	chunk_t* it = (chunk_t *)&end;
    chunk_t* ch = it;

    static uint32 reserved;
    static uint32 free;
    printf("============================\n");
    for (; ch; ch = ch->next) {
        printf("prt: %x, size: %d, %s\n",(char*)ch,ch->size,ch->free ? "FREE" : "RESERVED");
        /*if (ch->free){
        	free += ch->size;
        }else{
        	reserved += ch->size;
        }*/
    }

    //printf("FREE: %d RESERVED: %d\n",free,reserved );
    printf("============================\n\n");

#elif MALLOC_MODE == 4
printf("free %d\n",heap_free());

#endif
return 0;
}

int xsh_reboot(int nargs, char *args[])
{
  #define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   /*!< AIRCR Key for write access   */
  #define NVIC_SYSRESETREQ            2
  SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ)); /* Keep priority group unchanged */                                                                         /* Ensure completion of memory access */              
  while(1);                                    
  return 0;
}








int xsh_load(int nargs, char *args[]){


    exec_img ximg;
    int (*p)(int,char **);
    int ret = syscall_elf_execve(args[1],&ximg);
    
    p = (int *)ret;
    int child = syscall_create(p,STACK_SIZE, 1,args[1]);
	task_t *prptr = &task[child];
	prptr->elf = TRUE;
	prptr->img = (void *)ximg.start;
	strcpy(prptr->name,args[1]);
	context_t *ctx = (context_t*)prptr->sp;
	ctx->r0 = (uint32) nargs-1;
	ctx->r1 = (uint32)&args[1];
	syscall_ready(child);

	/*int pid = syscall_getpid();
	task_t *prptr = (task_t *)syscall_struct_task(pid);
	prptr->elf = TRUE;
	prptr->img = (void *)ximg.start;
	strcpy(prptr->name,args[1]);
	p = (int *)ret;
	p(nargs-1,&args[1]);*/


	
	return 0;
}

int xsh_elf(int nargs, char *args[]){
    
	/*exec_img ximg;
	program_img point;

    int (*p)(int,char **);

    for (int i = 0; i < nargs; ++i){
        dummy(args[i]);
    }

    FILE *fptr;
    if ((fptr = syscall_fopen(args[1],"r")) == NULL){
       	printf("Error! opening file");
        return -1;
    }

    uint8 *buff;
    syscall_fseek(fptr, 0, SEEK_END);
    int lsize = syscall_ftell(fptr);
    printf("size: %d\n",lsize );
    buff=syscall_malloc(lsize);
    syscall_fseek(fptr, 0, SEEK_SET);
  	syscall_fread(buff,lsize,1, fptr); 
    syscall_fclose(fptr); 

    point.img = (void*)buff;
    int r = load_elf(&point, &ximg);
    if(r!=0)panic("error load elf\n");
    syscall_free(buff);
  	*/


    exec_img ximg;
    int (*p)(int,char **);
    int ret = syscall_elf_execve(args[1],&ximg);

	int pid = syscall_getpid();
	task_t *prptr = (task_t *)syscall_struct_task(pid);
	prptr->elf = TRUE;
	prptr->img = (void *)ximg.start;
	strcpy(prptr->name,args[1]);
	p = (int *)ret;
	p(nargs-1,&args[1]);
	
    //uint32 mask=disable();
    //task_t *prptr;
    //prptr =& task[syscall_getpid()];
    //prptr->state=PR_FREE;
    //syscall_free(prptr->prstkbase);

	//int child = syscall_create((void *)ximg.entry,SHELL_CMDSTK, 1,args[1]);
	//task_t *prptr = &task[child];
	//context_t *ctx = (context_t*)prptr->sp;
	//ctx->r0 = (uint32) nargs-1;
	//ctx->r1 = (uint32)&args[1];
	//syscall_ready(child);

return 0;
}

#endif