

#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <stdarg.h>

extern int main();
extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/
extern	void testmem(void);

uint32 enable_tasks=0;
//char process[10];
//char count=0;
//PIC_STACK queue_list;
uint32 sys_time;
task_t *active_task=NULL;
queue_t ready_queue;
queue_t sleep_queue;
task_t task[MAX_NUMBER_OF_TASKS];
sem_t sem_task[MAX_NUMBER_OF_TASKS];

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
	/* for (int i = 0; i < count; ++i)
    {
    	syscall_ready(process[i]);
	    syscall_sleep(100);
    }*/
	while(1){
		PEND_SV();
	}
}



/*
static void blink(char *str){

	hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	for (;;)
	{
		printf("%s\n",str );
		hw_set_pin(GPIOx(GPIO_C), 13, 1);//pin_set(pin_c13);
		sleep(50);
		hw_set_pin(GPIOx(GPIO_C), 13, 0);//pin_reset(pin_c13);
		sleep(50);
	}
}*/



int starting(){

  



    uint32 start = get_timer(TIMER2);
	//syscall_attach_locks(sem_wait(&sem_fat32),sem_signal(&sem_fat32));
    if (syscall_sd_init() < 0){
	    panic("ERROR: Cannot init SD card\n");
	    //blink("ERROR: Cannot init SD card");
	    return -1;
	}

	// Initialise File IO Library
	syscall_fs_init();

	// Attach media access functions to library
	
	if (syscall_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
	{
	    panic("ERROR: Failed to init file system\n");
	    //blink("ERROR: Failed to init file system");
	    return -1;
	}

	listdirectory("/");

	FILE *fptr;
	exec_img ximg;

    char line[100];//=syscall_malloc(100);
	
	if ((fptr = syscall_fopen("/autorun.txt","r")) == NULL){
	   	panic("Error! opening autorun.txt");
	   	//blink("Error! opening autorun.txt");
	    return -1;
	}
	printf("\n");
	
	while (syscall_fgets(line, 100, fptr)) { 

	      line[strcspn(line, "\n")] = 0;

	       printf("->(%s)\n",line );
           //syscall_sema_wait(&sem_task[syscall_getpid()]);
	       int ret = syscall_elf_execve(line,&ximg);
		    if(ret != -1){
		        void (*p) = (int *)ret;
			    int child = syscall_create(p,STACK_SIZE, 1,line);
				task_t *prptr =& task[child];//(task_t *)syscall_struct_task(child);
				prptr->elf = TRUE;
				prptr->img = (void *)ximg.start;
				strcpy(prptr->name,line);
				//ready(child);
				//process[count]=child;
			    //count++;
			    syscall_ready(child);
			    syscall_sleep(100);
			    //syscall_sema_signal(&sem_task[syscall_getpid()]);
		    }else {
		    	printf("error loading process  %d\n",ret);
		    }
	}
	
	syscall_fclose(fptr); 
	//syscall_free(line);
    uint32 end = get_timer(TIMER2);
    printf("time %d %d %d\n",end-start,end,start );


   /* start = get_timer(TIMER2);
    for (int i = 0; i < 100; ++i)
    {
    	asm("nop");
    	asm("nop");
    	asm("nop");
    	asm("nop");
    	asm("nop");
    }
    end = get_timer(TIMER2);
    printf("time %d %d %d\n",end-start,end,start );*/

    //ready(create((int *)main,STACK_SIZE,1,"null1",0));
    return 0;
}

#endif
 


void	nulluser()
{	


enable();
init_systick(50,1);
init_sys();	 
uartinit();
init_mem();



sys_time = 0;
active_task = NULL;
for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
     task[i].state = PR_FREE;
     task[i].elf=FALSE;
     task[i].img=NULL;
     sem_init(&sem_task[i],1);
}

preempt=QUANTUM;
ntask=0;
queue_init(&ready_queue);
queue_init(&sleep_queue);
//StkInitialize(&queue_list);


kprintf ("Build date: %s %s %s\n\n", __DATE__, __TIME__,__FILE__);
SPI25lc1024();


ready(create((void *)nullprocess,128,1,"null0",0));
ready(create((int *)starting,STACK_SIZE,1,"null1",0)); 
//ready(create((int *)main,STACK_SIZE,1,"null1",0)); 
enable_tasks=1;


for(;;);
}
 
 
