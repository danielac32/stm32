#include <xinu.h>

//struct uart uarttab[1];

sem_t uart_tx;
sem_t uart_rx;
sem_t sem_malloc;
sem_t sem_syscall;
sem_t sem_gpio;
sem_t sem_fat32;
sem_t sem_load_elf;



void uartinit()
{
	struct	uart_csreg *uptr;	/* Address of UART's CSRs */
    init_sys();         /* Set MCU speed, enable preipherials, etc. */
    
    receive_buffer_head = 0;
    receive_buffer_tail = 0;
    
    sem_init(&uart_tx,1);
    sem_init(&uart_rx,1);
    sem_init(&sem_gpio,1);
    sem_init(&sem_syscall,1);
    sem_init(&sem_malloc,1);
    sem_init(&sem_fat32,1);
    sem_init(&sem_load_elf,1);
    hw_cfg_pin(GPIOx(GPIO_A),     11,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),     12,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	/* Enable peripheral */
	uptr = (struct uart_csreg *)(0x40011400);
	uptr->cr1 &= ~(1 << UART_EN);
	
	/* Set baudrate 115200 */
	uptr->brr = UART_BAUDRATE(100 MHZ, 115200);
	
	uptr->cr1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
	uptr->cr1 |= (1 << UART_EN);
    enable_interrupt(USART6_IRQn);

    timer_init(TIMER2, 8399, 1000);
    timer_init_int(TIMER2,  NULL);

}


 
void init_mem(){


kprintf("\nmalloc mode 1\n");
malloc_init(KMALLOC_START, KMALLOC_LENGTH);
malloc_debug();
testmem();


kprintf("%10d bytes of Xinu code.\n",
	(uint32)&etext - (uint32)&text);
kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)&text, (uint32)&etext - 1);
kprintf("%10d bytes of data.\n",
	(uint32)&ebss - (uint32)&data);
kprintf("           [0x%08X to 0x%08X]\n\n",
	(uint32)&data, (uint32)&ebss - 1);

}
