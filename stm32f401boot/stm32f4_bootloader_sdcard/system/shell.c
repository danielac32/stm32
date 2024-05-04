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
#include <math.h>
#include <usb.h>
#include "usb_cdc.h"
#include "usb-msc.h"
#include "sd-spi.h"
#include "disk.h"
#include "fat_filelib.h"
#include "st7735.h"
#include "gfx.h"
#include <shell.h>
#include <buttons.h>
#include <screen.h>
#include <list.h>



typedef char buf_t[128];
buf_t path, curdir = "/";


void setGlobalPath(){
    strcpy(path, "/");
    strcpy(curdir, path);
}
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

int  cd2(char *s){
 FL_DIR dirstat;
 //char *s=argv[1];
   if (strcmp(s, ".") == 0){
       goto cd_done;
   }else if (strcmp(s, "..") == 0) {
      if (strcmp(curdir, "/") == 0) {
          //kprintf("not a directory\n");
          //goto cd_done;//
          return -1;
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
       //kprintf("%s not found!\n",path );
       strcpy(path, curdir);
       return -1;
   }
   fl_closedir(&dirstat);
   //kprintf("%s\n",path);
    
   cd_done:
    strcpy(curdir, path);
    if (curdir[strlen(curdir) - 1] != '/'){
        strcat(curdir, "/");
    }
 return 0;
}

const   struct  cmdent  cmdtab[] = {
    {"cd",512,cd},
    {"ls",512,ls},
    {"cat",2048,cat},
    {"dump",2048,dump},
    {"echo",512,echo},
    {"help",512,help},

    {"mem",512, mem},
    {"mkdir",512,mkdirp},
   // {"ps",512,ps}, 
    {"pwd",512,pwd},
    {"rm",1024, rm},
    {"touch",512,touch},
    //{"run", 2048,run},
    {"format",1024,formatp},
    //{"load",1024,load},
};
uint32  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);



int  formatp(int argc, char *argv[]){
    if (fl_format(15625, "")){
         kprintf("format ok\n");
         return 0;
      }
    return 0;
}
int mem(int argc, char *argv[])
{ 
    uint32 memfree=heap_free();
    //uint32 fsfree=syscallp.disk_free();

    kprintf("mem free: %d\n", memfree);
    //kprintf("fs free: %d\n", fsfree);
    //kprintf("ok\n");
    return 0;
}


int  cd(int argc, char *argv[]){
 FL_DIR dirstat;
 char *s=argv[1];
   if (strcmp(s, ".") == 0){
       goto cd_done;
   }else if (strcmp(s, "..") == 0) {
      if (strcmp(curdir, "/") == 0) {
          kprintf("not a directory\n");
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
       kprintf("%s not found!\n",path );
       strcpy(path, curdir);
       return 0;
   }
   fl_closedir(&dirstat);
   //kprintf("%s\n",path);
    
   cd_done:
    strcpy(curdir, path);
    if (curdir[strlen(curdir) - 1] != '/'){
        strcat(curdir, "/");
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
                kprintf("%s <DIR>\r\n", dirent.filename);
            }
            else
            {
                kprintf("%s [%d bytes]\r\n", dirent.filename, dirent.size);
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
        kprintf("%s not found\n", tmp);
        return -1;
    }
    uint8 buff[32];
    //buff=syscallp.malloc(32);
    //char buff[32];
    int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    kprintf("size file %d\n",fileLength);

    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         fread(buff, 32,1,fd);
         hexDump(part,buff,32);
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
           kprintf("%s found!\n",tmp );
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
            kprintf("%s not found\n", tmp);
            return -1;
        }
        char buff[64];
        while(!feof(fd)){
              //fread(buff,64,1, fd);
              //kprintf("%s",buff );
              //memset(buff,0,64);
              char c = fgetc(fd);
              putchar(c);
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
    kprintf("%s ", argv[i]);
  }
  kprintf("\n");
    
	return 0;
}


 
int mkdirp(int argc, char *argv[])
{
    FL_DIR dirstat;
    char *tmp=full_path((char*)argv[1]);
    if(fl_opendir(tmp,&dirstat)){
        fl_closedir(&dirstat);
        kprintf("dir found %s\n",tmp);
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
        kprintf("not found %s\n",tmp);
        return -1;
    }
    remove(tmp);
	return 0;
}
int pwd(int argc, char *argv[])
{

    int i;
    char *s=full_path("");
    kprintf("%s\n",s );
	return 0;
}


int touch(int argc, char *argv[])
{

    FILE* fd;
    char *tmp=full_path((char*)argv[1]);
    if ((fd = fopen(tmp,"r"))){
        kprintf("file found %s\n",tmp);
        fclose(fd);
        return -1;
    }
    fd = fopen(tmp,"w");
    fclose(fd);
  return 0;
}


 

 

 

int help(int nargs, char *args[]){
for (int j = 0; j < ncmd; j++) {
     kprintf("%s\n",cmdtab[j].cname );
}

return 0;
}



#if 1
 
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

   // exec_img ximg;
	//int (*p)(int,char **);
	void *fptr;

    setGlobalPath();
    //if (curdir[strlen(curdir) - 1] != '/'){
      //  strcat(curdir, "/");
    //}
    /*********************************************************/
    /*********************************************************/
    char *val;
    int ulen;
    while(1){
//uint32 q = irq_disable();
        kprintf(SHELL_PROMPT);
        
        /*do{
           sleep(0);
        }
		while(len<=1);

		if (len <= 1) {
			continue;
		}
        memcpy(buf,&fifo,len);*/
        while(!usart_available()){
               //PEND_SV();
        }
        while(usart_available()){
              ulen = usart_readBytes(buf,sizeof(buf));
        }

		buf[ulen] = SH_NEWLINE;
        ntok = lexan(buf, ulen, tokbuf, &tlen, tok, toktyp);
        if (ntok == SYSERR) {
			kprintf("%s\n", SHELL_SYNERRMSG);
			continue;
		}
        if (ntok == 0) {
			kprintf("\n");
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
			kprintf(SHELL_SYNERRMSG);
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
            kprintf("command %s not found\n", tokbuf);
            continue;
        }
        kprintf("\n");
        for (i=0; i<ntok; i++) {
            args[i] = &tokbuf[tok[i]];
        }

        if (backgnd == FALSE){
            cmdtab[j].cfunc(ntok, args);
        }else{
           /* int child = create(cmdtab[j].cfunc, cmdtab[j].stack, 3, args[0],0);
            task_t *prptr=&task[child];   
            //prptr->elf = FALSE;
            //prptr->img = (void *)ximg.start;
            context_t *ctx = (context_t*)prptr->sp;
            ctx->r0 = (uint32) ntok;
            ctx->r1 = (uint32)&args[0];
            ready(child);
              //irq_restore(q);
            continue;*/
        }
    }
}
#endif


