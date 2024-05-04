

#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <stdarg.h>


extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/
extern	void testmem(void);
 

//PIC_STACK queue_list;
uint32 sys_time;
task_t *active_task=NULL;
queue_t ready_queue;
queue_t sleep_queue;
task_t task[MAX_NUMBER_OF_TASKS];
uint8 preempt;
uint8 ntask;
extern sem_t sem_fat32;
//sem_t test;

#if 1
int  shell(int argc,char *argv[]);




void listdirectory(char *path)
{
    FL_DIR dirstat;
    //FL_LOCK(&_fs);
   // uint32 q=disable();
    printf("\r\nDirectory %s\r\n", path);

    if (syscall_opendir(path, &dirstat))
    {
        struct fs_dir_ent dirent;

        while (syscall_readdir(&dirstat, &dirent) == 0)
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

        syscall_closedir(&dirstat);
    }
     //FL_UNLOCK(&_fs);
   // restore(q);
}


void nullprocess(){
	while(1){
		PEND_SV();
	}
}



void starting(){


	//syscall_attach_locks(sem_wait(&sem_fat32),sem_signal(&sem_fat32));
    if (syscall_sd_init() < 0){
	    printf("ERROR: Cannot init SD card\n");
	    return -1;
	}

	// Initialise File IO Library
	syscall_fs_init();

	// Attach media access functions to library
	
	if (syscall_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
	{
	    printf("ERROR: Failed to init file system\n");
	    return -1;
	}

	listdirectory("/");

	printf("loading shell...\n");
	ready(create((int *)shell,STACK_SIZE*3,0,"shell1",0)); 
	//kill(getpid());
	/*if (syscall_sd_init() < 0){
	    printf("ERROR: Cannot init SD card\n");
	    return -1;
	}

	// Initialise File IO Library
	syscall_fs_init();

	// Attach media access functions to library
	
	if (syscall_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
	{
	    printf("ERROR: Failed to init file system\n");
	    return -1;
	}
	// List the root directory
	syscall_listdirectory("/");
	//fl_listdirectory("/");*/
	
   // while(1){
    //	PEND_SV();
   // }

	printf("kernel at %08x\n",0x08020000 );
}

#endif
 



void	nulluser()
{	

init_sys();	 
uartinit();
init_mem();


//mount_fs();


sys_time = 0;
active_task = NULL;
for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
     task[i].state = PR_FREE;
     task[i].elf=FALSE;
     task[i].img=NULL;
}

preempt=QUANTUM;
ntask=0;
queue_init(&ready_queue);
queue_init(&sleep_queue);
//StkInitialize(&queue_list);


//sem_init(&test,0);
/*
if (sd_init() < 0){
    panic("ERROR: Cannot init SD card\n");
    return -1;
}

// Initialise File IO Library
fl_init();

// Attach media access functions to library
if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
{
    panic("ERROR: Failed to init file system\n");
    return -1;
}

fl_listdirectory("/");


ready(create((void *)nullprocess,CMCM_STACK_SIZE,1,"null0",0));
ready(create((void *)blink_task,CMCM_STACK_SIZE,1,"null",0));
ready(create((void *)blink_task1,CMCM_STACK_SIZE,1,"null",0));

enable();
init_systick(50,1);
*/

/*
exec_img ximg;
program_img point;

int (*p)();


FILE *fptr;
if ((fptr = syscall_fopen("/getpin2.elf","r")) == NULL){
	kprintf("Error! opening file");
return -1;
}

uint8 *buff;
syscall_fseek(fptr, 0, SEEK_END);
int lsize = syscall_ftell(fptr);
kprintf("size: %d\n",lsize );
buff=syscall_malloc(lsize);
syscall_fseek(fptr, 0, SEEK_SET);
syscall_fread(buff,lsize,1, fptr); 
syscall_fclose(fptr); 

point.img = (void*)buff;
int r = load_elf(&point, &ximg);
if(r!=0)panic("error load elf\n");
syscall_free(buff);


p = (int *)ximg.entry;
p();
*/

#if 1
 
ready(create((void *)nullprocess,STACK_SIZE,1,"null",0)); 
ready(create((void *)starting,STACK_SIZE*3,1,"null",0)); 
//ready(create((int *)shell,STACK_SIZE*3,0,"shell",0)); 
enable();
init_systick(50,1);

#endif

for(;;);
}
 
 
