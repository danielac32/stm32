#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <memory.h>
#include <kernel.h>
#include <delay.h>
#include <gpio.h>
#include <uart.h>
#include <process.h>
#include <fat_filelib.h>
#include <flash.h>
#include <usb_cdc_conf.h>
#include <shell.h>
#include <elf.h>
#include <stdlib.h>

 


extern int lib_putc(int ch);
typedef char buf_t[128];
buf_t path, curdir = "/";

void update_path(){
	strcpy(path, curdir);
}

char* full_path(const char* name) {
    if (name == NULL)
        return NULL;
    if (name[0] == '/') {
        strcpy(path, name);
        return path;
    }
    if (strncmp(name, "./", 2) == 0)
        name += 2;
    strcpy(path, curdir);
    if (strncmp(name, "../", 3) != 0) {
        if (name[0])
            strcat(path, name);
    } else {
        name += 3; // root doen't have a parent
        char* cp = strrchr(path, '/');
        if (cp != NULL)
            *cp = 0;
        cp = strrchr(path, '/');
        if (cp != NULL)
            *(cp + 1) = 0;
        strcat(path, name);
    }
    return path;
}


 
int  cd(int argc, char *argv[]);
int  ls(int argc, char *argv[]);
int  cat(int argc, char *argv[]);
int  dump(int argc, char *argv[]);
int  echo(int argc, char *argv[]);
int  help(int argc, char *argv[]);
int  killp(int argc, char *argv[]);
int  mem(int argc, char *argv[]);
int  mkdirp(int argc, char *argv[]);
int  ps(int argc, char *argv[]);
int  pwd(int argc, char *argv[]);
int  rm(int argc, char *argv[]);
int  touch(int argc, char *argv[]);
int  run(int argc, char *argv[]);
int  formatp(int argc, char *argv[]);
int load(int argc, char *argv[]);



 


extern int cc(int argc, char *argv[]);

int test(int argc, char *argv[]){
    

    printf("malloc\n");
    char *textc4 = malloc(10000);
    char *datac4 = malloc(1000);
    char *stack = malloc(1000);
    char *symbols = malloc(3000);

    printf("free\n");
    free(textc4);
    free(datac4);
    free(stack);
    free(symbols);
    return 0;
}

const   struct  cmdent  cmdtab[] = {
    {"cd",512,cd},
    {"ls",512,ls},
    {"cat",2048,cat},
    {"dump",2048,dump},
    {"echo",512,echo},
    {"help",512,help},
    {"kill",512,killp},
    {"mem",512, mem},
    {"mkdir",512,mkdirp},
    {"ps",512,ps}, 
    {"pwd",512,pwd},
    {"rm",1024, rm},
    {"touch",512,touch},
    {"run", 2048,run},
    {"format",1024,formatp},
    {"cc",4096*2,cc},
    {"test",1024,test},
    //{"load",1024,load},
};
uint32  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);



int  formatp(int argc, char *argv[]){
    if (fl_format(31250, "")){
         printf("format ok\n");
         return 0;
      }
    return 0;
}
int mem(int argc, char *argv[])
{ 
    uint32 memfree=heap_free();
    //uint32 fsfree=syscallp.disk_free();

    printf("mem free: %d\n", memfree);
    //printf("fs free: %d\n", fsfree);
    //printf("ok\n");
    return 0;
}


int  cd(int argc, char *argv[]){
 FL_DIR dirstat;
 char *s=argv[1];
   if (strcmp(s, ".") == 0){
       goto cd_done;
   }else if (strcmp(s, "..") == 0) {
      if (strcmp(curdir, "/") == 0) {
          printf("not a directory\n");
          //goto cd_done;//
          return 0;
      }
      strcpy(path, curdir);
      char* cp = strrchr(path, '/');
        if (cp == NULL)
            cp = curdir;
        *cp = 0;
        cp = strrchr(path, '/');
        if (cp != NULL)
            *(cp + 1) = 0;
        goto cd_done;
   }
   full_path(s);

   if(!fl_opendir(path,&dirstat)){
       printf("%s not found!\n",path );
       strcpy(path, curdir);
       return 0;
   }
   fl_closedir(&dirstat);
   //printf("%s\n",path);
    
   cd_done:
    strcpy(curdir, path);
    if (curdir[strlen(curdir) - 1] != '/'){
        strcat(curdir, "/");
    }
 return 0;
}




int ps(int argc, char *argv[]){
	char *pstate[]  = {     
        "free ", "curr ", "ready", "recv ", "sleep", "susp ",
        "wait "};
    printf("%3s %-16s %5s %10s %10s %10s\n",
               "Pid", "Name", "State", "Stack Base",
               "Stack Ptr", "Stack Size");
    

            for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
                task_t *prptr =&task[i];
                if (prptr->state == PR_FREE) {  
                    continue;
                }
                printf("%3d %-16s %5s 0x%08X 0x%08X %8d\n",
                    i, prptr->name, pstate[(int)prptr->state],
                    prptr->prstkbase,(uint32)prptr->sp, prptr->prstklen);
            }
return 0;
}


int ls(int argc, char *argv[]){
    FL_DIR dirstat;
    char *tmp=full_path("");
    if (fl_opendir(tmp, &dirstat))
    {
        struct fs_dir_ent dirent;

        while (fl_readdir(&dirstat, &dirent) == 0)
        {
            if (dirent.is_dir)
            {
                printf("%s <DIR>\r\n", dirent.filename);
            }
            else
            {
                printf("%s [%d bytes]\r\n", dirent.filename, dirent.size);
            }
        }

        fl_closedir(&dirstat);
    }

    return 0;
}

int dump(int argc, char *args[])
{

     FILE* fd;

    char *tmp=full_path(args[1]);
    if (!(fd = fopen(tmp,"r"))){
        printf("%s not found\n", tmp);
        return -1;
    }
    uint8 buff[32];
    //buff=syscallp.malloc(32);
    //char buff[32];
    int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    printf("size file %d\n",fileLength);

    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         fread(buff, 32,1,fd);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
    }
    fclose(fd);


    update_path();
    
    return 0;
}

int cat(int argc, char *argv[])
{
    FILE* fd;

 
    if(argc <2)return -1;
    if (!strcmp(argv[argc-2],">")){

        char *tmp=full_path((char*)argv[argc-1]);
        if (tmp==NULL)return -1;
        //if(disk.exist(tmp)){
        fd = fopen(tmp, "w");
        if(!fd){
           printf("%s found!\n",tmp );
           update_path();//strcpy(path, curdir);
           return 0;
        }
        for (int i = 1; i < argc-2; ++i)
        {
            fputs(argv[i],fd);
            fputs(" ",fd);
        }
        fclose(fd);
    }else{


        char *tmp=full_path((char*)argv[argc-1]);

        if (!(fd = fopen(tmp,"r"))){
            printf("%s not found\n", tmp);
            return -1;
        }
        char buff[64];
        while(!feof(fd)){
              //fread(buff,64,1, fd);
              //printf("%s",buff );
              //memset(buff,0,64);
              char c = fgetc(fd);
              lib_putc(c);
        }
        fclose(fd);
    }
    update_path();//strcpy(path, curdir);

    return 0;
}


int echo(int argc, char *argv[])
{
    int i;

  for (int i = 1; i < argc; ++i)
  {
    printf("%s ", argv[i]);
  }
  printf("\n");
    
	return 0;
}


int killp(int argc, char *argv[])
{
 
    int i;

  if(argc < 2){
    printf("usage: kill pid...\n");
    return -1;
  }
  for(i=1; i<argc; i++)
    kill(atoi(argv[i]));
    
	return 0;
}

int mkdirp(int argc, char *argv[])
{
    FL_DIR dirstat;
    char *tmp=full_path((char*)argv[1]);
    if(fl_opendir(tmp,&dirstat)){
        fl_closedir(&dirstat);
        printf("dir found %s\n",tmp);
        return 0;
    }
    
    //mkdir(tmp);
    return 0;
}

int rm(int argc, char *argv[])
{
    FILE* fd;
    char *tmp=full_path((char*)argv[1]);
    if (!(fd = fopen(tmp,"r"))){
        printf("not found %s\n",tmp);
        return -1;
    }
    remove(tmp);
	return 0;
}
int pwd(int argc, char *argv[])
{

    int i;
    char *s=full_path("");
    printf("%s\n",s );
	return 0;
}


int touch(int argc, char *argv[])
{

    FILE* fd;
    char *tmp=full_path((char*)argv[1]);
    if ((fd = fopen(tmp,"r"))){
        printf("file found %s\n",tmp);
        fclose(fd);
        return -1;
    }
    fd = fopen(tmp,"w");
    fclose(fd);
  return 0;
}



 
 


int run(int nargs, char *args[]){
    char *tmp=full_path((char*)args[1]);
    FILE *fptr;
    exec_img ximg;
    printf("file : %s\n",tmp);
    int ret = elf_execve(tmp,&ximg);
    if(ret > 0){
        int (*p) = (int *)ret;
        //int pid = syscallp.create((void *)p, 4096, 3, args[1]);
        int pid = create((void *)p, 2048, 3, tmp, 0);
        task_t *prptr=& task[pid];
        //task_t *prptr=disk.self(pid); 
        prptr->elf = TRUE;
        prptr->img = (void *)ximg.start;
        context_t *ctx = (context_t*)prptr->sp;
        ctx->r0 = (uint32) nargs-1;
        ctx->r1 = (uint32)&args[1];
        ready(pid);
    }else {
        printf("error loading elf process  %d %s\n",ret,tmp);
    }

    update_path();
    return 0;
}


 

int help(int nargs, char *args[]){
for (int j = 0; j < ncmd; j++) {
     printf("%s\n",cmdtab[j].cname );
}

return 0;
}



#if 1
struct jsonframe
{
    uint32 len;
    uint32 size;
    char *buffer;
    char *val;
    char *find;
};

void shell(){
     
 
   // path_shell=Qcreate();
    char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	//int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
				SHELL_MAXTOK];  /* Contiguous null-terminated	*/
	int32	tlen;			/* Current length of all data	*/
						/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	//int	child;			/* Process ID of spawned child	*/
	bool	backgnd;		/* Run command in background?	*/
		
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
		
	char	*src, *cmp;		/* Pointers used during name	*/
						/*   comparison			*/
	bool diff;			/* Was difference found during	*/
	char	*args[SHELL_MAXTOK];

    exec_img ximg;
	int (*p)(int,char **);
	void *fptr;

    strcpy(path, "/");
    strcpy(curdir, path);
    //if (curdir[strlen(curdir) - 1] != '/'){
      //  strcat(curdir, "/");
    //}
    /*********************************************************/
    /*********************************************************/
    char *val;
    int ulen;
    while(1){
//uint32 q = irq_disable();
        printf(SHELL_PROMPT);
        
        /*do{
           sleep(0);
        }
		while(len<=1);

		if (len <= 1) {
			continue;
		}
        memcpy(buf,&fifo,len);*/
        while(!usb_available()){
               //PEND_SV();
               sleep(0);
        }
        while(usb_available()){
              ulen = readBytes(buf,sizeof(buf));
        }

		buf[ulen] = SH_NEWLINE;
        ntok = lexan(buf, ulen, tokbuf, &tlen, tok, toktyp);
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
        printf("\n");
        for (i=0; i<ntok; i++) {
            args[i] = &tokbuf[tok[i]];
        }

        if (backgnd == FALSE){
            cmdtab[j].cfunc(ntok, args);
        }else{
            int child = create(cmdtab[j].cfunc, cmdtab[j].stack, 3, args[0],0);
            task_t *prptr=&task[child];   
            //prptr->elf = FALSE;
            //prptr->img = (void *)ximg.start;
            context_t *ctx = (context_t*)prptr->sp;
            ctx->r0 = (uint32) ntok;
            ctx->r1 = (uint32)&args[0];
            ready(child);
              //irq_restore(q);
            continue;
        }
    }
}
#endif

