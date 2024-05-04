#include <xinu.h>


/*
 * The reset handler is the first executable code to run after
 * startup. It is defined at the end of the file, and performs
 * basic memory initialization, then calls the main() function.
 */
void reset_handler();
extern void nulluser();
void default_handler();
#define STACK_TOP	0x20010000

extern void usart6_irq();
extern void tim9_handler(void);
extern void SysTick_Handler(void);
extern void __attribute__ ((naked)) _svc_handler(void);
extern void __attribute__ ((naked)) resched(void);
extern void clkhandler();
extern void ttyhandler(uint32 xnum);
extern void svccall_handler_s();
extern void SVC_Handler();

unsigned int _vectors[] __attribute__ ((section(".vectors"))) = {
	
	// processor exceptions
	(unsigned int)STACK_TOP,		   // 	stack top
	(unsigned int)&reset_handler,	   // 	reset handler
    (unsigned int)&default_handler,    //	NMI_Handler
	(unsigned int)&default_handler,    //	HardFault_Handler
	(unsigned int)&default_handler,    //	MemManage_Handler
	(unsigned int)&default_handler,    //	BusFault_Handler
	(unsigned int)&default_handler,    //	UsageFault_Handler
	0,
	0,
	0,
	0,
	(unsigned int)&_svc_handler,    //	SVC_Handler
	(unsigned int)&default_handler,    //	DebugMon_Handler
	0,
	(unsigned int)&resched,    //	PendSV_Handler
	(unsigned int)&clkhandler,    //	SysTick_Handler
	
	// interrupts
	(unsigned int)&default_handler,                    /* Window WatchDog              */
    (unsigned int)&default_handler,                     /* PVD through EXTI Line detection */
    (unsigned int)&default_handler,              /* Tamper and TimeStamps through the EXTI line */
    (unsigned int)&default_handler,                /* RTC Wakeup through the EXTI line */
    (unsigned int)&default_handler,                   /* FLASH                        */
    (unsigned int)&default_handler,                     /* RCC                          */
    (unsigned int)&default_handler,                   /* EXTI Line0                   */
    (unsigned int)&default_handler,                   /* EXTI Line1                   */
    (unsigned int)&default_handler,                   /* EXTI Line2                   */
    (unsigned int)&default_handler,                   /* EXTI Line3                   */
    (unsigned int)&default_handler,                   /* EXTI Line4                   */
    (unsigned int)&default_handler,            /* DMA1 Stream 0                */
    (unsigned int)&default_handler,            /* DMA1 Stream 1                */
    (unsigned int)&default_handler,            /* DMA1 Stream 2                */
    (unsigned int)&default_handler,            /* DMA1 Stream 3                */
    (unsigned int)&default_handler,            /* DMA1 Stream 4                */
    (unsigned int)&default_handler,            /* DMA1 Stream 5                */
    (unsigned int)&default_handler,            /* DMA1 Stream 6                */
    (unsigned int)&default_handler,                     /* ADC1, ADC2 and ADC3s         */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,                 /* External Line[9:5]s          */
    (unsigned int)&default_handler,           /* TIM1 Break and TIM9          */
    (unsigned int)&default_handler,           /* TIM1 Update and TIM10        */
    (unsigned int)&default_handler,      /* TIM1 Trigger and Commutation and TIM11 */
    (unsigned int)&default_handler,                 /* TIM1 Capture Compare         */
    (unsigned int)&default_handler,                    /* TIM2                         */
    (unsigned int)&default_handler,                    /* TIM3                         */
    (unsigned int)&default_handler,                    /* TIM4                         */
    (unsigned int)&default_handler,                 /* I2C1 Event                   */
    (unsigned int)&default_handler,                 /* I2C1 Error                   */
    (unsigned int)&default_handler,                 /* I2C2 Event                   */
    (unsigned int)&default_handler,                 /* I2C2 Error                   */
    (unsigned int)&default_handler,                    /* SPI1                         */
    (unsigned int)&default_handler,                    /* SPI2                         */
    (unsigned int)&default_handler,                  /* USART1                       */
    (unsigned int)&default_handler,                  /* USART2                       */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,               /* External Line[15:10]s        */
    (unsigned int)&default_handler,               /* RTC Alarm (A and B) through EXTI Line */
    (unsigned int)&default_handler,             /* USB OTG FS Wakeup through EXTI line */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,            /* DMA1 Stream7                 */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,                    /* SDIO                         */
    (unsigned int)&default_handler,                    /* TIM5                         */
    (unsigned int)&default_handler,                    /* SPI3                         */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,            /* DMA2 Stream 0                */
    (unsigned int)&default_handler,            /* DMA2 Stream 1                */
    (unsigned int)&default_handler,            /* DMA2 Stream 2                */
    (unsigned int)&default_handler,            /* DMA2 Stream 3                */
    (unsigned int)&default_handler,            /* DMA2 Stream 4                */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&OTG_FS_IRQHandler,                  /* USB OTG FS                   */
    (unsigned int)&default_handler,            /* DMA2 Stream 5                */
    (unsigned int)&default_handler,            /* DMA2 Stream 6                */
    (unsigned int)&default_handler,            /* DMA2 Stream 7                */
    (unsigned int)&ttyhandler,                  /* USART6                       */
    (unsigned int)&default_handler,                 /* I2C3 event                   */
    (unsigned int)&default_handler,                 /* I2C3 error                   */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler,                     /* FPU                          */
    0,                                  /* Reserved                     */
    0,                                  /* Reserved                     */
    (unsigned int)&default_handler                     /* SPI4                         */

};

/*
 * Reset handler implementation, the first and only piece of code the hardware itself
 * automatically calls on bootup.
 */
void reset_handler()
{
	/*
	 * We first want to copy the values in .data to RAM, as that
	 * is where we told our code to look for them in the linker 
	 * script. 
	 */
	unsigned int* psrc = &etext;	// start at the end of .text, where .data is loaded in flash
	unsigned int* pdest = &data;	// copy to start of .data, where it is linked to in ram
	
	while (pdest < &edata) {
		// copy the value from flash to ram
		*pdest = *psrc;
		++psrc; 
		++pdest;
	}
	
	
	/*
	 * Next set the values in the .bss section in ram to zeros. Some standard somewhere
	 * guarantees this section to be zeros, thus we will oblige without confrontation.
	 */
	pdest = &bss;		// start at the beginning of .bss in ram
	while (pdest < &ebss)
	{
		// set each value to zero
		*pdest = 0x0;
		++pdest;
	}
	
    SystemInit();
   
  

 /*    
 printf("bytes of code : %10d \n",(unsigned int)&_etext - (unsigned int)&_text);
 printf("              [0x%08X to 0x%08X]\n",(unsigned int)&_text, (unsigned int)&_etext);
 printf("bytes of data : %10d \n",(unsigned int)&_edata - (unsigned int)&_sdata);
 printf("              [0x%08X to 0x%08X]\n\n",(unsigned int)&_sdata, (unsigned int)&_edata);
*/
	nulluser();
}

/*
 * Our default handler (the handler all interrupts run at this point) does
 * nothing more than stall the processor in an infinite loop. If you are 
 * looking to implement a specific interrupt, then create a new function and 
 * poin`t to it in the corresponding row in the vector table. However, the 
 * processor will boot just fine as is.
 */
void default_handler()
{
	uint32 * current_sp;

   asm volatile("\
        tst lr, #4; \
        ite eq; \
        mrseq %0, msp; \
        mrsne %0, psp; \
    " : "=r" (current_sp));


    for (int i = 0; i < 15; i++) {
        kprintf("STACK[%d]: %x\n", i, current_sp[i]);
    }
    
    halt(); 
}




