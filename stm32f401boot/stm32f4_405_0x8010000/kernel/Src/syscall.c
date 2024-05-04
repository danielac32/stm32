#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stm32.h>
#include <usb_cdc_conf.h>
#include <queue.h>
#include <memory.h>
#include <kernel.h>
#include <process.h>
#include <syscall.h>
#include <elf.h>
#include <gpio.h>

syscall_t *sys;
sem_t sem_syscall;

void* sys_open(const char *path, int flags){
    return (void *)syscall(XINU_FOPEN,path,flags);
}
int sys_exist(const char *path){
    return syscall(XINU_EXIST,path);
}
int sys_write(void *pbuf, int size, void *file){
    return syscall(XINU_FWRITE,pbuf,size,file);
}
int sys_read(void *pbuf, int size, void *file){
    return syscall(XINU_FREAD,pbuf,size,file);
}
int sys_close(void *file){
    return syscall(XINU_FCLOSE,file);
}
int sys_seek(void *file, int offset, int whence){
    return syscall(XINU_FSEEK,file,offset,whence);
}
int sys_tell(void *file){
    return syscall(XINU_FTELL,file);
}
int sys_rewind(void *file){
    return syscall(XINU_FREWIN,file);
}
int sys_truncate(int size, void *file){
    return syscall(XINU_TRUNCATE,size,file);
}
int sys_size(void *file){
    return syscall(XINU_SIZE,file);
}
int sys_available(void *file){
    return syscall(XINU_AVAILABLE,file);
}

int sys_mkdir(const char *path){
    return syscall(XINU_MKDIR,path);
}
void *sys_openDir(const char *path){
    return (void *)syscall(XINU_OPENDIR,path);
}
int sys_closeDir(void *dir){
    return syscall(XINU_CLOSEDIR,dir);
}
int sys_readDir(void *dir, void *info){
    return syscall(XINU_READDIR,dir,info);
}
int sys_rewindDir(void *dir){
    return 0;
}
int sys_tellDir(void *dir){
    return 0;
}
int sys_seekDir(void *dir, int offset){
    return 0;
}

int sys_format(void *_lfs, void *_cfg){
    return syscall(XINU_FORMAT,_lfs,_cfg);
}
int sys_mount(void *_lfs, void *_cfg){
    return syscall(XINU_MOUNT,_lfs,_cfg);
}
int sys_unmount(void *_lfs){
    return syscall(XINU_UNMOUNT,_lfs);
}
int sys_remove(const char *path){
    return syscall(XINU_REMOVE,path);
}
int sys_rename(const char *oldpath, const char *newpath){
    return syscall(XINU_RENAME,oldpath,newpath);
}
int sys_free(void){
    return syscall(XINU_DISK_FREE);
}
void sys_sleep(uint32 ticks){
    syscall(XINU_SLEEP,ticks);
}

void sys_kill(int pid){
    syscall(XINU_KILL,pid);
}

int sys_getpid(){
    return syscall(XINU_GETPID);
}

void sys_yield(){
    syscall(XINU_YIELD);
}

void sys_ready(int pid){
    syscall(XINU_READY,pid);
}

char *sys_full_path(char *s){
    return (char *)syscall(XINU_GLOBAL_PATH,s);
}
void sys_update_path(){
    syscall(XINU_UPDATE_PATH);
}

void *sys_malloc(uint32 size){
    return (void *)syscall(XINU_MALLOC,size);
}

void sys_mfree(void *p){
     syscall(XINU_FREE,p);
}

void sys_putc(char c){
    syscall(XINU_PUTC,c);
}
void sys_puts(char *s){
    syscall(XINU_PUTS,s);
}

void *sys_struct_task(int pid){
return (void *)syscall(XINU_STRUCT_TASK,pid);
}

char *sys_get_path(){
    return (char *)syscall(XINU_GET_PATH);
}

void sys_cd(char *s){
    syscall(XINU_CD,s);
}

uint32 sys_elf_execve(const char *file, void *res){
       return syscall(XINU_LOAD_ELF,file,res);
}

int sys_create(void *procaddr, uint32 ssize, int priority,const char *name){
    return syscall(XINU_CREATE,procaddr,ssize,name);
}

void *sys_js0n(void *frame){
    return syscall(XINU_JSON,frame);
}

uint32 sys_free_heap(){
    return syscall(XINU_FREE_HEAP);
}


uint32 len_fifo_usb(){
        return syscall(XINU_GET_LEN);
}
char *fifo_usb(){
       return (char *)syscall(XINU_GETS);
}




int syscall_init(syscall_t *sys_obj){
    sys = sys_obj;
    sys->exist = sys_exist;
    sys->open = sys_open;
    sys->write = sys_write;
    sys->read = sys_read;
    sys->close = sys_close;
    sys->seek = sys_seek;
    sys->tell = sys_tell;
    sys->rewind = sys_rewind;
    sys->truncate = sys_truncate;
    sys->size = sys_size;
    sys->available = sys_available;

    sys->mkdir = sys_mkdir;
    sys->openDir = sys_openDir;
    sys->closeDir = sys_closeDir;
    sys->readDir = sys_readDir;
    sys->rewindDir = sys_rewindDir;
    sys->tellDir = sys_tellDir;
    sys->seekDir = sys_seekDir;

    sys->format = sys_format;
    sys->mount = sys_mount;
    sys->unmount = sys_unmount;
    sys->remove = sys_remove;
    sys->rename = sys_rename;
    sys->disk_free = sys_free;
    sys->sleep = sys_sleep;
    sys->kill = sys_kill;
    sys->getpid = sys_getpid;
    sys->yield = sys_yield;
    sys->ready = sys_ready;
    //sys->create = 0;
    sys->reset = 0;
    sys->putc = sys_putc;
    sys->puts = sys_puts;
    sys->kbhit = 0;
    sys->readbyte = 0;
    sys->full_path = sys_full_path;
    sys->updatepath = sys_update_path;
    sys->malloc = sys_malloc;
    sys->free = sys_mfree;
    sys->self = sys_struct_task;
    sys->getpath = sys_get_path;
    sys->cd = sys_cd;
    sys->elf_execve=sys_elf_execve;
    sys->create = sys_create;
    sys->js0n = sys_js0n;
    sys->freeHeap = sys_free_heap;
    
    sys->len=len_fifo_usb;
    sys->fifo=fifo_usb;
    return 0;
}

extern void update_path();
extern char* full_path(const char* name);
extern void cd(char *s);



char *loadConfig(char *path,int *size){
/*if(!disk.exist(path)){
    printf("error init shell\n");
    return NULL;
}
FILE *fs=disk.open(path,LFS_O_RDONLY);
int ssize = disk.size(fs);
char *jsonbuff=malloc(ssize+1);
disk.read(jsonbuff,ssize,fs);
disk.close(fs);
*size=ssize;
return jsonbuff;*/
    return 0;
}


//const char *js0n(const char *key, size_t klen,
  //               const char *json, size_t jlen, size_t *vlen)

/*
struct jsonframe
{
    uint32 len;
    uint32 size;
    char *buffer;
    char *val;
    char *find;
};*/

void svccall_handler(uint32 *sp) {
SYS_ENTRY();
uint32 svc_nr = sp[0];
task_t *prptr;
switch(svc_nr) {
       case XINU_FREE_HEAP:
            {
                sp[0]=heap_free();
            }
        break;
        /*case XINU_JSON:
           {
             //sp[0]=js0n((char *)sp[1],sp[2],(char *)sp[3],sp[4],(size_t *)sp[5]);
              struct jsonframe *json_t = (struct jsonframe *)sp[1];
              int len;
              json_t->val = js0n(json_t->find,0,json_t->buffer,json_t->size,&len);
              json_t->len=len;
              sp[0]=(uint32)json_t;
            //
           }*/
        break;
        case XINU_LOAD_ELF:
             sp[0]=elf_execve((char *)sp[1],(exec_img *)sp[2]);
        break;
        case XINU_CD:
            {
             cd((char *)sp[1]);
            }
        break;
        case XINU_SIZE:
             //sp[0]=disk.size((FILE *)sp[1]);
             
        break;
        case XINU_GET_PATH:
            sp[0]=full_path("");
            
        break;
        case XINU_STRUCT_TASK:
            sp[0]=&task[sp[1]];
            
        break;
        case XINU_UPDATE_PATH:
            {
                update_path();
            }
        break;
        case XINU_GLOBAL_PATH:
            {
                sp[0]=full_path((char *)sp[1]);   
            }
        break;

        case XINU_EXIST:
             //sp[0]=disk.exist((char *)sp[1]);
             
        break;
        case XINU_YIELD:
             //ctx->pc =(uint32)&resched;
        break;   
        case XINU_SLEEP:
            sleep(sp[1]);
            
        break;
        case XINU_FREE:
            free((void *)sp[1]);
            
        break;
        case XINU_MALLOC:
            sp[0] = malloc(sp[1]);
            
        break;

        case XINU_KILL:
             kill(sp[1]);
             
        break;
        case XINU_NULLPROCESS:
            {
                /*int len;
                int size;
                char jsonp[100];
                char *jsonbuff=loadConfig("shell/shell.json",&size);
                char *val;

                memset(jsonp,0,100);
                val = js0n("loader",0,jsonbuff,size,&len);
                memcpy(jsonp,val,len);
                exec_img ximg;
                int (*p)(int,char **);
                int ret =elf_execve(jsonp,&ximg);
                if(ret == -1){
                    panic("error load\n");
                }
                p = (int *)ret;
                int child = create(p, 4096, 3, "shell", 0);
                task_t *prptr=&task[child];//syscallp.self(child);  
                prptr->elf = TRUE;
                prptr->img = (void *)ximg.start;
                ready(child);*/
                /*exec_img ximg;
                int (*p)(int,char **);
                int ret =elf_execve("boot/loader.elf",&ximg);
                if(ret == -1){
                    panic("error load\n");
                }
                p = (int *)ret;
                int child = create(p, 4096, 3, "shell", 0);

                task_t *prptr=& thrtab[child];
                prptr->elf = TRUE;
                prptr->img = (void *)ximg.start;
                ready(child);*/
            }
        case XINU_PUTC:
            //printf("%c",sp[0]);
            {
               // sem_wait(&uart_tx);
                //UART=sp[0];
                /*while(1){
                    int len = usbd_ep_write(&udev, CDC_TXD_EP, &sp[1], 1);
                    if (len < 0){
                        continue;
                    }else break;
                }*/
                while(usbd_ep_write(&udev, CDC_TXD_EP, &sp[1], 1)==-1);
               // sem_signal(&uart_tx);
            }
            //csr_put(sp[0]);
            
            //ctx->pc =(uint32)&start;

        break;
        break;
        case XINU_PUTS:
            {
                char *str = (char *)sp[1];
                int size = sp[2];
                sem_wait(&uart_usb);
           // asm volatile("cpsid i");
                while(usbd_ep_write(&udev, CDC_TXD_EP, str, size)==-1);
    //asm volatile("cpsie i");
                sem_signal(&uart_usb);
             //printf("%s", (char *)sp[1]);
            }
           // sp[0] = 0;
            
        break;
        
        case XINU_GETC:
            {
                sp[0]=fifo[0];
            }
        break;
        case XINU_GET_LEN:
             {
                sp[0]=len;
             }
        break;
        case XINU_GETS:
            {
                sp[0]=&fifo[0];
            }
        break;
        case XINU_CLOSE:
            {

            }
        break;
        case XINU_OPEN:
            {

            }
        break;
        case XINU_READ:
            {

            }
        break;
        case XINU_WRITE:
            {

            }
        break;
        case XINU_CREATE:
             sp[0]=create((void *)sp[1], sp[2], 3, (char *)sp[3], 0);
             
        break;
        case XINU_READY:
            ready(sp[1]);
            
        break;
        case XINU_SEMA_INIT:

        break;
        case XINU_SEMA_SIGNAL:

        break;
        case XINU_SEMA_WAIT:

        break;

        case XINU_FOPEN:
            {
                //sp[0] = disk.open((char *)sp[1],sp[2]);
                
            }
        break;
        case XINU_FSEEK:
            {
                //sp[0] = disk.seek((FILE *)sp[1],sp[2],sp[3]);
                
            }
        break;
        case XINU_AVAILABLE:
            {
                //sp[0] = disk.available((FILE *)sp[1]);
                
            }
        break;
        case XINU_FREAD:
            {
                //sp[0] = disk.read((void *)sp[1],sp[2],(FILE *)sp[3]);
                
            }
        break;
        case XINU_FWRITE:
            {
                //sp[0] = disk.write((void *)sp[1],sp[2],(FILE *)sp[3]);
                
            }
        break;
        case XINU_FCLOSE:
            {
                //sp[0] = disk.close((FILE *)sp[1]);
                
            }
        break;
        case XINU_FTELL:
                {
                    //sp[0] = disk.tell((FILE *)sp[1]);
                    
                }
        break;
        case XINU_FEOF:

        break;
        case XINU_REMOVE:
            {
                //sp[0] = disk.remove((char *)sp[1]);
                
            }
        break;
        case XINU_OPENDIR:
            {
                //sp[0]=disk.openDir((char *)sp[1]);
            }
        break;
        case XINU_READDIR:
            {
                //sp[0] = disk.readDir((DIR *)sp[1],(struct lfs_info *)sp[2]);
                
            }
        break;
        case XINU_CLOSEDIR:
            {
                //sp[0] = disk.closeDir((DIR *)sp[1]);
                
            }
        break;
        case XINU_MKDIR:
            {
                //sp[0] = disk.mkdir((char *)sp[1]);
                
            }
        break;
        case XINU_RENAME:
            {
                //sp[0] = disk.rename((char *)sp[1],(char *)sp[2]);
                
            }
        break;
        case XINU_FORMAT:
            {
                //sp[0] = disk.format((lfs_t *)sp[1],(struct lfs_config *)sp[2]);
                
            }
        break;
        case XINU_MOUNT:
            {
                //sp[0] = disk.mount((lfs_t *)sp[1],(struct lfs_config *)sp[2]);
                
            }
        break;
        case XINU_UNMOUNT:
            {
                //sp[0] = disk.unmount((lfs_t *)sp[1]);
                
            }
        break;
        case XINU_DISK_FREE:
            {
                //sp[0] = disk.free();
                
            }
        break;
        case XINU_IS_DIR:

        break;
        case XINU_FFLUSH:

        break;
        case XINU_FGETC:

        break;
        case XINU_FGETCS:

        break;
        case XINU_FPUTC:

        break;
        case XINU_FPUTCS:

        break;
        case XINU_FGETPOS:

        break;
        case XINU_GETPID:
            {
                sp[0]=getpid();
            }
        break;
        case XINU_NUM_TASK:
            sp[0]=MAX_NUMBER_OF_TASKS;
        break;
        case XINU_PIN_MODE://pin mode
        {
            GPIO_TypeDef *p;
            p = (GPIO_TypeDef *)sp[1];
            hw_cfg_pin(p,sp[2],sp[3]);//hw_cfg_pin((GPIO_TypeDef*)args[1],    args[2],     args[3]);
         }
        break;
        case XINU_PIN_SET://pin set
             {
                GPIO_TypeDef *p;
                p = (GPIO_TypeDef *)sp[1];
                hw_set_pin(p, sp[2],sp[3]);
                //kprintf("pin set %d %d\n", sp[2],sp[3]);
             }
        break;
        case XINU_PIN_GET://pin mode
         {
            GPIO_TypeDef *p;
            p = (GPIO_TypeDef *)sp[1];
            sp[0] = hw_get_pin(p,sp[2]);
            //kprintf("pin get %d %d\n",sp[2],ret);
         }
        break;

        default:
            kprintf("syscall no implement, %d\n",svc_nr);
            
        break;
}
//kprintf("syscall  %d %d\n",sp[0],sp[1]);
//sp[0]=0xbebecafe;
SYS_EXIT();
}