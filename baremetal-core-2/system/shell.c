
#include <xinu.h>
#include <elf.h>



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
        	 //printf("\n");
        	 //child = create(cmdtab[j].cfunc,SHELL_CMDSTK, SHELL_CMDPRIO,cmdtab[j].cname,0);//
        	 child = syscall_create(cmdtab[j].cfunc,SHELL_CMDSTK, SHELL_CMDPRIO,cmdtab[j].cname);
			 task_t *prptr = &task[child];
			 cmcm_stack_frame_t *ctx = (cmcm_stack_frame_t*)prptr->sp;
			 ctx->hw_frame.r0 = (uint32) ntok;
			 ctx->hw_frame.r1 = (uint32)&args;
			 //context_t *ctx = (context_t*)prptr->sp;
			 //ctx->r0 = (uint32) ntok;
			 //ctx->r1 = (uint32)&args;
	         syscall_ready(child);
	         //printf("%s %d\n",cmdtab[j].cname,child );
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
	syscall_ready(atoi(args[1]));
	return 0;
}
int xsh_bin(int nargs, char *args[]){
	FILE *fptr;

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
    p(0,NULL);
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

   	extern int load_elf2( void *prg,char *out);

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
    
    int r = load_elf2(buff,"/a.out");
    if(r!=0)printf("error elf\n");
    syscall_free(buff);


    if ((fptr = syscall_fopen("/a.out","r")) == NULL){
	   	printf("Error! opening file");
	    return -1;
    }
    syscall_fseek(fptr, 0, SEEK_END);
    lsize = syscall_ftell(fptr);
    printf("size: %d\n",lsize );
    buff=syscall_malloc(lsize);
    syscall_fseek(fptr, 0, SEEK_SET);
  	syscall_fread(buff,lsize,1, fptr); 
    syscall_fclose(fptr); 
    syscall_remove("/a.out");
    int pid = syscall_getpid();
	task_t *prptr = (task_t *)syscall_struct_task(pid);
	prptr->elf = TRUE;
	prptr->img = buff;
	strcpy(prptr->name,args[1]);
    int (*p)(int,char ** )= (int *)((uintptr_t)buff | 1);
    p(nargs-1,&args[1]);
	return 0;
}



#if 0
int read_ELF_section_header(int32_t sect_num,ELF32_shdr *sect_hdr,FIL *fp)
{
	int32_t numsect;
	ELF32_hdr elfhdr;
	uint32 sect_hdr_off;
	uint32 l;
	f_lseek(fp,0);
	f_read(fp,&elfhdr, sizeof(ELF32_hdr),&l );

	numsect=elfhdr.e_shnum;
	if ((numsect<sect_num)||(sect_num<0))
		return -1;
	sect_hdr_off=elfhdr.e_shoff;
	//rewind(fp);
	sect_hdr_off+=elfhdr.e_shentsize*sect_num;
	f_lseek(fp,sect_hdr_off);
	f_read(fp, sect_hdr, sizeof(ELF32_shdr),&l );
	return 1;	
}


void process_sect_hdr(ELF32_shdr *sect_hdr,char *sect_name_buff,int i)
{
	int32_t idx;
	idx=sect_hdr->sh_name;
	printf(" %-20s ",sect_name_buff+idx);
	printf(" addr: %08x ", sect_hdr->sh_addr);
	printf(" size: %08x ", sect_hdr->sh_size);
	printf(" offset: %08x \n", sect_hdr->sh_off);

	/*if(i==1){//text
	   text.addr=sect_hdr->sh_addr;
	   text.offset=sect_hdr->sh_offset;
	   text.size=sect_hdr->sh_size;
	}else if (i==2){//rodata
	   rodata.addr=sect_hdr->sh_addr;
	   rodata.offset=sect_hdr->sh_offset;
	   rodata.size=sect_hdr->sh_size;
	}else if (i==3){//data
	   data.addr=sect_hdr->sh_addr;
	   data.offset=sect_hdr->sh_offset;
	   data.size=sect_hdr->sh_size;
	}else if (i==4){//sdata
	   sdata.addr=sect_hdr->sh_addr;
	   sdata.offset=sect_hdr->sh_offset;
	   sdata.size=sect_hdr->sh_size;
	}else if (i==5){//bss
	   bss.addr=sect_hdr->sh_addr;
	   bss.offset=sect_hdr->sh_offset;
	   bss.size=sect_hdr->sh_size;
	}*/
}

uint32 get_size(uint32 num,FIL *fp){
uint32 size=0;
ELF32_shdr sec_hdr;
for(int i=0;i<num;i++){
	if(read_ELF_section_header(i,&sec_hdr,fp)==-1)
	{
		printf("Wrong Section to read\n");
	}
	else
	{
		size += sec_hdr.sh_size;
	}
}
return size;
}
#endif


#if 0
int xsh_elf(int nargs, char *args[]){

	FIL file;
	uint32 lsize,l=0;
	uint8 *memory;
	FRESULT fr;
    int (*p)(int,char **);

    fr = f_stat(args[1], NULL);
    if(fr != FR_OK){
      printf("FILE NOT FOUND\n");
      return SYSERR;
    }
    if (f_open(&file, args[1], FA_OPEN_ALWAYS | FA_READ | FA_WRITE)==FR_OK) {
        lsize = f_size(&file);

        ELF32_hdr hdr;
        ELF32_shdr sec_hdr;
        int num_hdrs;
        char buff[64];
        char *p=&buff;

        f_read(&file, &hdr, sizeof(ELF32_hdr),&l );
        if (!is_elf_x(&hdr)) {
            printf("E_NOT_ELF_X\n");
	    }
	    if (!is_compatible(&hdr)) {
	        printf("E_NOT_COMPATIBLE\n");
	    }
	    num_hdrs=hdr.e_shnum;
	   // printf("jump app %x\n",hdr.e_entry);
	    if(read_ELF_section_header(hdr.e_shstrndx,&sec_hdr,&file)==-1)
	    {
			printf("Error: reading section string table sect_num= %d\n",hdr.e_shstrndx);
			return 0;
	    }
	    uint32 size = get_size(num_hdrs,&file);
	    printf("size image: %d\n",size );

        void* to_addr = (void *)syscall_malloc(size);

	    printf("%d\n",hdr.e_entry);
	    f_lseek(&file,sec_hdr.sh_off);
	    f_read(&file, buff, sec_hdr.sh_size,&l );
	    printf("There are [%d] sections\n",num_hdrs);
	
	    for(int i=1;i<num_hdrs;i++){
			if(read_ELF_section_header(i,&sec_hdr,&file)==-1)
			{
				printf("Wrong Section to read\n");
			}
			else
			{
				printf("[section %3d] ",i);
				process_sect_hdr(&sec_hdr,p,i);
				//uintptr_t daddr = to_addr + sec_hdr.sh_addr;
				char *name = p + sec_hdr.sh_name;

				/*if(sec_hdr.sh_size>0){
					uint32 *src = (uint32 *)syscall_malloc(sec_hdr.sh_size);
					f_lseek(&file,sec_hdr.sh_off);
	                f_read(&file, src, sec_hdr.sh_size,&l );
					
					hexDump(0,name, src,sec_hdr.sh_size);

					syscall_free(src);
				}*/

				if(sec_hdr.sh_size>0){
					if(strncmp(name,".got",4)==0){

						/*uint32 *src = (uint32 *)syscall_malloc(sec_hdr.sh_size);
						f_lseek(&file,sec_hdr.sh_off);
		                f_read(&file, src, sec_hdr.sh_size,&l );

		                //hexDump(0,name, src,sec_hdr.sh_size);
		                
		                uint32 *dest = (uint32*)daddr;
		                while ((uintptr_t)dest < (uint32)(daddr + sec_hdr.sh_size)) {
		                    *dest++ = to_addr + *src++; // to_addr is also the offset (as start is at 0x0)
		                }
		                syscall_free(src);*/
					}else if(!strncmp(name,".text",5)){
							{
		                         
                        		uint32 *src = (uint32 *)syscall_malloc(sec_hdr.sh_size);
								f_lseek(&file,sec_hdr.sh_off);
				                f_read(&file, src, sec_hdr.sh_size,&l );
		                        memcpy((void*)to_addr, (void*)src, sec_hdr.sh_size);
		                      
		                        //hexDump(0,name, src,sec_hdr.sh_size);
		                        syscall_free(src);
		                        	
		                    }
		            }
				}

				/*if(strncmp(name,".got",4)==0){
					uint32 *src  = (uint32*)(sec_hdr.sh_off);
	                uint32 *dest = (uint32*)daddr;
	                while ((uintptr_t)dest < (uint32)(daddr + sec_hdr.sh_size)) {
	                    *dest++ = to_addr + *src++; // to_addr is also the offset (as start is at 0x0)
	                }
				}else{
					switch (sec_hdr.sh_type) {
	                    case SHT_NOBITS: // .bss
	                        memset((void*)daddr, 0, sec_hdr.sh_size);
	                        break;
	                    case SHT_PROGBITS: // .data, .rodata, .text (should use FLAGS to set rwx perms)
	                        memcpy((void*)daddr, (void*)(sec_hdr.sh_off), sec_hdr.sh_size);
	                        break;
                    }
				}
				*/

			}
		}

        hexDump(0,"", to_addr,size);
        int (*app)()=(void*)((uintptr_t)to_addr | 0x1);
        app();

        f_close(&file);
    }else printf("error file\n");
return 0;
}
#else
int xsh_elf(int nargs, char *args[]){
    
	exec_img ximg;
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
  	

	int pid = syscall_getpid();
	task_t *prptr = (task_t *)syscall_struct_task(pid);
	prptr->elf = TRUE;
	prptr->img = ximg.start;
	strcpy(prptr->name,args[1]);
	p = (int *)ximg.entry;
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