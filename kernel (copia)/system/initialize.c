/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* initialize.c - nulluser, sysinit */

/* Handle system initialization and become the null process */

#include <xinu.h>
//#include <string.h>
//#include <syscall.h>


extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/
extern 	int32 initintc(void);
//extern int sys__call(int num,...);
//void startup(int, ...);		/* Process to finish startup tasks	*/


/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/
//bool start_null=false;
/* Control sequence to reset the console colors and cusor positiion	*/

#define	CONSOLE_RESET	" \033[0m\033[2J\033[;H"


unsigned char mem[]={
 0x80, 0xb5, 0x00, 0xaf, 0x4f, 0xf4, 0x64, 0x72, 0x0d, 0x21, 0x03, 0x20, 0x00, 0xf0, 0x1f, 0xf8, 
 0x01, 0x22, 0x0d, 0x21, 0x04, 0x20, 0x00, 0xf0, 0x1a, 0xf8, 0xc8, 0x21, 0x09, 0x20, 0x00, 0xf0, 
 0x16, 0xf8, 0x00, 0x22, 0x0d, 0x21, 0x04, 0x20, 0x00, 0xf0, 0x11, 0xf8, 0x64, 0x21, 0x09, 0x20, 
 0x00, 0xf0, 0x0d, 0xf8, 0xec, 0xe7, 0xef, 0xf3, 0x03, 0x80, 0x72, 0xb6, 0x70, 0x47, 0x80, 0xf3, 
 0x03, 0x88, 0x62, 0xb6, 0x70, 0x47, 0x62, 0xb6, 0x70, 0x47, 0x72, 0xb6, 0xfe, 0xe7, 0x00, 0xdf, 
 0xc0, 0x46, 0x70, 0x47, 0x64, 0xdf, 0x70, 0x47,                        };






 


/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */




/*

void blink_syscall(){


	__syscall2(0,13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	
	while(1){
		__syscall3(0,13, 1);
		__syscall4(0,200);

		__syscall3(0,13, 0);
		__syscall4(0,100);

	    int ret = __syscall5(0,33,2);
	    printf("%d\n", ret);
	}
}


void blink(){
    hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	for (;;)
	{
		hw_set_pin(GPIOx(GPIO_C), 13, 1);//pin_set(pin_c13);
		sleep(100);
		hw_set_pin(GPIOx(GPIO_C), 13, 0);//pin_reset(pin_c13);
		sleep(200);
	}
}

void button(){

    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
	//xinu_syscall(XINU_PIN_MODE,GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
	
	while(1)
	{
		if(!hw_get_pin(GPIOx(GPIO_A),0)){
		//if(!xinu_syscall(XINU_PIN_GET,GPIOx(GPIO_A),0)){
			printf("key button ok\n");
			sleep(200);
		}
		printf("!\n");
		sleep(2000);
	}
}*/

//static void printf3(const int code=88,char *ftm,...){

//} 
 
 


void nullprocess(void) {
	//resume(create(shell, 4096/2, 50, "shell", 1, 0));

	__syscall(XINU_TEST2,shell,4096/2, "shell");
	//resume(create((void *)mem, 1024/2, INITPRIO, "blink", 0, NULL));
	//resume(create((void *)blink_syscall, 1024/2, INITPRIO, "blink", 0, NULL));
	//resume(create((void *) , 1024/2, INITPRIO, "button", 0, NULL));
	//resume(create((void *)blink, 1024/2, INITPRIO, "blink", 0, NULL));
	
    

    //resume(create((void *)main, 4096*2, INITPRIO, "elf", 0, NULL));
	
	
	
    //uint32 r = 0xbebecafe;//read_flash(VARADDR);
    //printf("1) %08x \n", r);
    //printf2("2) %08x \n", r);
	while(1){
     
	}
}


 



void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/

	/* Initialize the system */
    
	sysinit();

	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)&text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&text, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&ebss - (uint32)&data);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&data, (uint32)&ebss - 1);

     
	/* Initialize the Null process entry */	
	int pid = create((void *)nullprocess, 256, 10, "Null process", 0, NULL);
	struct procent * prptr = &proctab[pid];
	prptr->prstate = PR_CURR;



    FATFS FatFs;
    FRESULT res;
	DIR dirs;
	FILINFO Finfo;


	if ((res = f_mount(&FatFs, "", 1)) != FR_OK) {
	    kprintf("mount error! res=%d\n", res);
	    return;//return 1;
	}
	kprintf("mount successfully\n");



	/* Enable interrupts */
	enable();
	
	/* Initialize the real time clock */
	clkinit();
	
	/* Start of nullprocess */
	//startup(XINU_NULLPROCESS, prptr);
    
    //sys__call(XINU_NULLPROCESS,prptr);

 
    /*asm volatile ("mov r0, %0\n" : : "r" (prptr->prstkptr));
	asm volatile ("msr psp, r0");
	asm volatile ("ldmia r0!, {r4-r11} ");
    asm volatile ("msr psp, r0");
	asm volatile ("mov r0, #2");
	asm volatile ("msr control, r0");
	asm volatile ("isb");
    nullprocess();*/

	//asm volatile ("msr psp, r0");
	//asm volatile ("ldr r0, =0xFFFFFFFD" "\n\t"
			  //"mov lr, r0" "\n\t"
			  //"bx lr");


	/*asm volatile("\
		mov r0, 0; \
		mov r1, %0; \
		svc 1; \
	" :: "r" (prptr));*/
    

   


    __syscall(XINU_NULLPROCESS,prptr);



	for(;;);

}

/* Startup does a system call, the processor switches to handler 
 * mode and prepares for executing the null process (see syscall.c) 
 * This is also where a kernel mode to user mode switch can
 * take place */


/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */


 
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/


	/* Platform Specific Initialization */

	platinit();

	//kprintf(CONSOLE_RESET);
	for (int i = 0; i < 10; ++i)
	{
		kprintf("\n");
	}
	/* Initialize free memory list */
	
	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */
	prcount = 0;
//	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}


	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	
	readylist = newqueue();

	
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}


/*
int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
	;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}

 */
void delay_ms(uint32 s)
{
    for(s; s>0; s--){
		// Reset the timer
		TIM3->EGR |= 0x0001;
		// Wait until timer reaches to 1000
		// It is 1000 becuase timer is running at 1 MHz and 1000 will
		//   generate 1 milli-second
 		while(TIM3->CNT < 1000);
	}
}


void OTG_FS_IRQHandler(void)
{
  //USBD_OTG_ISR_Handler(&g_usb_dev);
}
