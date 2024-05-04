#include <syscall.h>
#include <kernel.h>

int syscall_putc(int c) {
   __syscall(XINU_PUTC, c);
   return OK;
}
int syscall_getc() {
   return __syscall(XINU_GETC);
}

int syscall_sema_init(void *sem,int value){
  //sem_init(sem,value);
  return __syscall(XINU_SEMA_INIT,sem,value);
}
int syscall_sema_signal(void *sem){
    //sem_signal(sem);
    return __syscall(XINU_SEMA_SIGNAL,sem);
}
int syscall_sema_wait(void *sem){
  //sem_wait(sem);
    return __syscall(XINU_SEMA_WAIT,sem);
}
int syscall_ready(int pid){
    return __syscall(XINU_READY,pid);
}

void* syscall_malloc(int size){
    return (void *)__syscall(XINU_MALLOC,size);
}

int syscall_free(void* p){
    return __syscall(XINU_FREE,p);
}
int syscall_pin_mode(void* p,int pin,int cfg){
    return __syscall(XINU_PIN_MODE,p,pin,cfg);
}
int syscall_pin_set(void* p,int pin,int state){
    return __syscall(XINU_PIN_SET,p,pin,state);
}
int syscall_pin_get(void* p,int pin){
    return __syscall(XINU_PIN_GET,p,pin);
}
int syscall_kill(int pid){
    return __syscall(XINU_KILL,pid);
}
int syscall_sleep(int delay){
    return __syscall(XINU_SLEEP,delay);
}
int syscall_create(int (*procaddr)(),int ssize,int priority,char *name){
  return __syscall(XINU_CREATE,procaddr,ssize, priority,name);
}
int syscall_getpid(){
  return __syscall(XINU_GETPID);
}
//int syscall_read() {
  // return __syscall(XINU_READ);
//}

void *syscall_struct_task(int pid){
return (void *)__syscall(XINU_STRUCT_TASK,pid);
}

int syscall_num_task(){
  return __syscall(XINU_NUM_TASK);
}



int syscall_sd_init(void){
  return __syscall(XINU_SD_INIT);
}
void  syscall_fs_init(void){
 __syscall(XINU_FS_INIT);
}


int   syscall_attach_media(void *w, void *s){
  return __syscall(XINU_ATTACH_MEDIA,w,s);
}

void   syscall_attach_locks(void *rd, void *wr){
   __syscall(XINU_ATTACH_LOCKS,rd,wr);
}


void* syscall_fopen(char *path,char *modifiers){
  return (void *) __syscall(XINU_FOPEN,path,modifiers);
}

int   syscall_fseek(void *file , long offset , int origin ){
  return __syscall(XINU_FSEEK,file,offset,origin);
}

int   syscall_fread(void * data, int size, int count, void *file ){
  return __syscall(XINU_FREAD,data,size,file);
}

void  syscall_fclose(void *file){
  __syscall(XINU_FCLOSE,file);
}
long  syscall_ftell(void *f){
   return __syscall(XINU_FTELL,f);
}

int   syscall_feof(void *f){
return __syscall(XINU_FEOF,f);
}
int   syscall_remove(char * filename){
return __syscall(XINU_REMOVE,filename);
}
void* syscall_opendir(char* path, void *dir){
return (void*)__syscall(XINU_OPENDIR,path,dir);
}
int  syscall_readdir(void *dirls, void *entry){
return __syscall(XINU_READDIR,dirls,entry);
}
int  syscall_closedir(void* dir){
return __syscall(XINU_CLOSEDIR,dir);
}
int   syscall_createdirectory(char *path){
  return __syscall(XINU_CREATEDIRECTORY,path);
}
int   syscall_is_dir(char *path){
  return __syscall(XINU_IS_DIR,path);
}


int   syscall_fflush(void *file){
  return __syscall(XINU_FFLUSH,file);
}
int   syscall_fgetc(void *file){
  return __syscall(XINU_FGETC,file);
}
char *  syscall_fgets(char *s, int n, void *f){
  return (char *)__syscall(XINU_FGETCS,s,n,f);
}
int   syscall_fputc(int c, void *file){
  return __syscall(XINU_FPUTC,c,file);
}
int   syscall_fputs(char * str, void *file){
  return __syscall(XINU_FPUTCS,str,file);
}
int   syscall_fwrite(void * data, int size, int count, void *file ){
  return __syscall(XINU_FWRITE,data,size,file);
}
int   syscall_fgetpos(void *file , uint32 * position){
  return __syscall(XINU_FGETPOS,file,position);
}
