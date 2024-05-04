/* Xinu system calls */

//extern void __attribute__((naked))  svccall_handler_c(uint32 *sp);
extern int __syscall(int a,...);
//extern int __syscall2(int a,...);
//extern uint32 __attribute__((naked))xinu_syscall(int nr, uint32 arg1, uint32 arg2, uint32 arg3); 
extern int syscall_putc(int c);
extern int syscall_getc();
extern int syscall_kill(int);
extern int syscall_sema_init(void *,int);
extern int syscall_sema_signal(void *);
extern int syscall_sema_wait(void *);
extern int syscall_ready(int pid);
extern void* syscall_malloc(int size);
extern int syscall_free(void* p);
extern int syscall_pin_mode(void* p,int pin,int cfg);
extern int syscall_pin_set(void* p,int pin,int state);
extern int syscall_pin_get(void* p,int pin);
extern int syscall_kill(int pid);
extern int syscall_sleep(int delay);
extern int syscall0(int code);
extern int syscall1(int code, int arg1);
extern int syscall2(int code, int arg1, int arg2);
extern int syscall3(int code, int arg1, int arg2, int arg3);
extern int syscall4(int code, int arg1, int arg2, int arg3, int arg4);
extern int syscall_create(int (*procaddr)(),int ssize,int priority,char *name);
extern int syscall_getpid();

#if 0
extern int syscall_fopen(void *,char *,int);
extern int syscall_fclose(void *);
extern int syacall_fread(void *,void *,int,int *);
extern int syacall_fwrite(void *,void *,int,int *);
extern int syscall_flseek(void *,int);
extern int syscall_fopendir(void *,char *);
extern int syscall_fclosedir(void *);
extern int syscall_readdir(void *,void *);
extern int syscall_fmkdir(char *);
extern int syscall_funlink(char *);
extern int syscall_frename (char *, char *);
extern int syscall_fstat (char *,void *);
extern int syscall_fmount (void *,char *, int);
extern int syscall_fputc (char , void *);	
extern int syscall_fputs (char *, void *);								/* Put a string to the file */
extern int syscall_fprintf (void *, char *, ...);						/* Put a formatted string to the file */
extern char* syscall_fgets (char *, int, void *);	
extern int syscall_feof(void *);
extern int syscall_ferror(void *);
extern int syscall_ftell(void *);
extern int syscall_fsize(void *);
extern int syscall_frewind(void *); //f_lseek((fp), 0)
extern int syscall_frewinddir(void *); //f_readdir((dp), 0)
#endif

extern void *syscall_struct_task(int pid);
extern int syscall_num_task();

extern int syscall_sd_init(void);
extern void  syscall_fs_init(void);
extern void* syscall_fopen(char *path,char *modifiers);
extern void  syscall_fclose(void *file);
extern int   syscall_fflush(void *file);
extern int   syscall_fgetc(void *file);
extern char *  syscall_fgets(char *s, int n, void *f);
extern int   syscall_fputc(int c, void *file);
extern int   syscall_fputs(char * str, void *file);
extern int   syscall_fwrite(void * data, int size, int count, void *file );
extern int   syscall_fread(void * data, int size, int count, void *file );
extern int   syscall_fseek(void *file , long offset , int origin );
extern int   syscall_fgetpos(void *file , uint32 * position);
extern long  syscall_ftell(void *f);
extern int   syscall_feof(void *f);
extern int   syscall_remove(char * filename);
extern void* syscall_opendir(char* path, void *dir);
extern int  syscall_readdir(void *dirls, void *entry);
extern int  syscall_closedir(void* dir);

// Extensions
extern void  syscall_listdirectory(char *path);
extern int   syscall_createdirectory(char *path);
extern int   syscall_is_dir(char *path);
extern int   syscall_format(uint32 volume_sectors,char *name);
extern int   syscall_attach_media(void *rd, void *wr);
extern void  syscall_attach_locks(void *w, void *s);


enum {
XINU_NULLPROCESS = 0,

XINU_PUTC ,
XINU_GETC,
	//XINU_INIT,
XINU_CLOSE,  /* Devices */
XINU_OPEN ,
XINU_READ ,
XINU_WRITE ,

XINU_CREATE , /* Processes */ 
	//XINU_CHPRIO,
XINU_KILL, /* Exit */
	//XINU_GETPRIO,
XINU_READY ,

XINU_SLEEP ,
XINU_FREE ,
XINU_MALLOC, 

XINU_GETDEV ,

XINU_SEMA_INIT,  /* Semaphores */
XINU_SEMA_SIGNAL, 
XINU_SEMA_WAIT ,


XINU_PIN_MODE ,
XINU_PIN_SET ,
XINU_PIN_GET ,
XINU_KMALLOC_CHUNK, 
XINU_GETPID ,

XINU_SD_INIT ,
XINU_FS_INIT ,
XINU_ATTACH_MEDIA, 
XINU_ATTACH_LOCKS , //XINU_LIST_DIR 25
XINU_FOPEN,
XINU_FSEEK,
XINU_FREAD,
XINU_FWRITE,
XINU_FCLOSE,
XINU_FTELL,
XINU_FEOF,
XINU_REMOVE,
XINU_OPENDIR,
XINU_READDIR,
XINU_CLOSEDIR,
XINU_CREATEDIRECTORY,
XINU_IS_DIR,
XINU_FFLUSH,
XINU_FGETC,
XINU_FGETCS,
XINU_FPUTC,
XINU_FPUTCS,
XINU_FGETPOS,
XINU_NUM_TASK ,
XINU_STRUCT_TASK, 
};

extern sem_t sem_syscall;