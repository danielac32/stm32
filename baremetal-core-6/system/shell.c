#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <usbapi.h>
#include <queue.h>
#include <memory.h>
#include <kernel.h>
#include <littlefs.h>
#include <process.h>
#include <syscall.h>
#include <shell.h>
#include <elf.h>
#include <re.h>

//int len;
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
int  mkdir(int argc, char *argv[]);
int  ps(int argc, char *argv[]);
int  pwd(int argc, char *argv[]);
int  rm(int argc, char *argv[]);
int  touch(int argc, char *argv[]);
int  run(int argc, char *argv[]);
int  formatp(int argc, char *argv[]);
int load(int argc, char *argv[]);


const   struct  cmdent  cmdtab[] = {
    {"cd",512,cd},
    {"ls",512,ls},
    {"cat",2048,cat},
    {"dump",2048,dump},
    {"echo",512,echo},
    {"help",512,help},
    {"kill",512,killp},
    {"mem",512, mem},
    {"mkdir",512,mkdir},
    {"ps",512,ps}, 
    {"pwd",512,pwd},
    {"rm",1024, rm},
    {"touch",512,touch},
    {"run", 2048,run},
    {"format",1024,formatp},
    {"load",1024,load},
};
uint32  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);



int  formatp(int argc, char *argv[]){

    printf("unmount     ");
    int ret = disk.unmount(disk.lfs);
    if(ret) 
    {
        printf("[FAIL]\n");
        printf("Error Code: %d\n", ret);
        return -1;
    }
    printf("[ OK ]\n");
    printf("format    ");
    ret = disk.format(disk.lfs, &config);
    if(ret) 
    {
        printf("[FAIL]\n");
        printf("Error Code: %d\n", ret);
        return -1;
    }
    printf("[ OK ]\n");
   
    printf("mount     ");
    ret = disk.mount(disk.lfs, &config);
    if(ret) 
    {
        printf("[FAIL]\n");
        printf("Error Code: %d\n", ret);
        return -1;
    }

    printf("[ OK ]\n");
   uint32 disk_free = disk.free();
    uint32 freekb = disk_free/1000;
    //int end=cycleCount();
    //int res = end-start;
    //kprintf("in %d us \n",res);
    printf("available size = %d kb\n", freekb);
    delay(10);

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
   if(!disk.exist(path)){
       printf("%s not found!\n",path );
       strcpy(path, curdir);
       return 0;
   }
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
	struct lfs_info info;
    char *s=full_path("");

    if (s==NULL || !strcmp(s,"")) return 0;
    DIR *dir = disk.openDir(s);
    if(dir==0){
        printf("not a directory %s",s);
        return 0;
    }
    while (disk.readDir(dir, &info)> 0){
        if (strcmp(info.name, ".") && strcmp(info.name, "..") && strcmp(info.name, "//")){            
            char *tmp=full_path(info.name);
            if (tmp==NULL) return 0;
            if(disk.exist(tmp)){
                if (info.type == LFS_TYPE_DIR){
                    printf(" %s\n",info.name);
                }else{
                    printf(" %s %d \n",info.name,info.size);
                }
            }
            update_path();
        }
    }

    disk.closeDir(dir);
    return 0;
}


int cat(int argc, char *argv[])
{
	
	 
 
    if(argc <2)return -1;
	if (!strcmp(argv[argc-2],">")){

        char *tmp=full_path((char*)argv[argc-1]);
        if (tmp==NULL)return -1;
        printf("-> %s\n",tmp );
	    if(disk.exist(tmp)){
           printf("%s found!\n",tmp );
           update_path();//strcpy(path, curdir);
           return 0;
        }

        FILE *fs=disk.open(tmp,LFS_O_CREAT);
        if (fs < 0){
            return -1;
        }
        for (int i = 1; i < argc-2; ++i)
        {
        	disk.write(argv[i],strlen(argv[i]),fs);
        	disk.write(" ",1,fs);
        }
        disk.close(fs);
	}else{
		char *tmp=full_path((char*)argv[argc-1]);
	    if(!syscallp.exist(tmp)){
           printf("%s not found!\n",tmp );
           update_path();//strcpy(path, curdir);
           return 0;
        }
        FILE *fs=disk.open(tmp,LFS_O_RDWR);
        while(disk.available(fs)){
        	 char ch;
        	 disk.read(&ch,1,fs);
        	 printf("%c",ch );
        }
        disk.close(fs);
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

int dump(int argc, char *args[])
{

    char *tmp=full_path(args[1]);
    if (tmp==NULL)return -1;
	if(!syscallp.exist(tmp)){
       printf("%s not found!\n",args[1] );
       update_path();
       return -1;
    }
    uint8 buff[32];
    //buff=syscallp.malloc(32);
    //char buff[32];
    int part=0;
	FILE *fs=disk.open(tmp,LFS_O_RDWR);
	int size = disk.size(fs);
	printf("size file %d\n",size);
	while(part<size){
		 //syscallp.seek(fs,part,LFS_SEEK_SET);
         disk.read(buff,32,fs);
         hexDump2(part,buff,32);
         memset(buff,0,32);
         part += 32;
	}
    disk.close(fs);
    //syscallp.free(buff);


    update_path();

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

int mkdir(int argc, char *argv[])
{
 
  int r;
  if (argc < 2)return -1;
  
  if(argv[1]==NULL)return -1;

  char *tmp=full_path(argv[1]);
  if(disk.exist(tmp)){
    printf("%s dir found!\n",tmp );
    update_path();
    return -1;
  }
  
  if((r=disk.mkdir(tmp))<0){
       printf("error (%d)\n",r);
       update_path();
       return -1;
  }
  update_path();

 
  return 0;
}

int rm(int argc, char *argv[])
{
 
  int r;

  if(argc < 2){
     
    return -1;
  }
  if(argv[1]==NULL)return -1; 

  char *tmp=full_path(argv[1]);
  if(!disk.exist(tmp)){
    printf("%s not found!\n",tmp );
    update_path();
    return -1;
  }
  
  if((r=disk.remove(tmp))<0){
       printf("error (%d)\n",r);
       update_path();
       return -1;
  }
  printf("%s\n",tmp );
  update_path();
    
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

  int r;
  if(argc < 2){
    return -1;
  }
  if(argv[1]==NULL)return -1;

  char *tmp=full_path(argv[1]);
  if(disk.exist(tmp)){
    printf("%s file found!\n",tmp );
    update_path();
    return -1;
  }
  
  FILE *fs=disk.open(tmp,LFS_O_CREAT);
  disk.close(fs);
  update_path();
  return 0;
}



int load(int argc, char *argv[])
{
  int r;
  if(argc < 2){
    return -1;
  }
  if(argv[1]==NULL)return -1;
  
  printf("waiting...\n");
  char *tmp=full_path(argv[1]);
  if(disk.exist(tmp)){
    kprintf("%s file found!\n",tmp );
    update_path();
    return -1;
  }

//  int size_malloc=atoi(argv[i]);
  uint8 *buff=malloc(40000);
  int count=0;

  while(1){
      do{
        if (!hw_get_pin(GPIOx(GPIO_A),0)){
            goto label_end_load;
        }
        //sleep(0);
      }
      while(!usb_available());
      //
      char c=(char)usb_getc();
      lib_putc2(c);
      //kprintf("%02x",c);
      buff[count]=c;
      count++;
  }
 
  label_end_load:
  printf("writing..\n");
  //hexDump2(0,buff,count);
  FILE *fs=disk.open(tmp,LFS_O_CREAT);
  disk.write(buff,count,fs);
  disk.close(fs);
  update_path();
  free(buff);

  return 0;
}

#if 0
int load(int argc, char *argv[])
{
  int r;
  if(argc < 2){
    return -1;
  }
  if(argv[1]==NULL)return -1;
  
  printf("waiting...\n");
  char *tmp=full_path(argv[1]);
  if(disk.exist(tmp)){
    kprintf("%s file found!\n",tmp );
    update_path();
    return -1;
  }

//  int size_malloc=atoi(argv[i]);
  uint8 *buff=malloc(40000);
  int count=0;

  while(1){
      do{
        if (!hw_get_pin(GPIOx(GPIO_A),0)){
            goto label_end_load;
        }
        //sleep(0);
      }
      while(len<0);
      //
      char c=fifo[0];
      kprintf("%02x",c);
      buff[count]=c;
      count++;
  }
 
  label_end_load:
  printf("writing..\n");
  //hexDump2(0,buff,count);
  FILE *fs=disk.open(tmp,LFS_O_CREAT);
  disk.write(buff,count,fs);
  disk.close(fs);
  update_path();
  free(buff);

  return 0;
}
#endif


int run(int nargs, char *args[]){
 
    char *tmp=full_path(args[1]);
    if(!disk.exist(tmp)){
       printf("%s not found!\n",args[1] );
       strcpy(path, curdir);
       return 0;
    }
    FILE *fptr;
    exec_img ximg;
    int ret = elf_execve(tmp,&ximg);

    if(ret != -1){
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
        printf("error loading process  %d\n",ret);
    }

    strcpy(path, curdir);
    return 0;
}


uint8_t readBytes2(char *buffer,uint8_t length){
  uint8_t count = 0;
  while (count < length) {
    while(!usb_available());
    int c = usb_getc();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}


 
 

int help(int nargs, char *args[]){
for (int j = 0; j < ncmd; j++) {
     printf("%s\n",cmdtab[j].cname );
}



while(1){
    while(!usb_available());
    char c = usb_getc();
    lib_putc2(c);
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

