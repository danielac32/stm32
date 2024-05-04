#include <xinu.h>

//struct uart uarttab[1];

sem_t uart_tx;
sem_t uart_rx;
sem_t sem_malloc;
sem_t sem_syscall;
sem_t sem_gpio;
sem_t sem_fat32;

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
    hw_cfg_pin(GPIOx(GPIO_A),     11,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),     12,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	/* Enable peripheral */
	uptr = (struct uart_csreg *)(0x40011400);
	uptr->cr1 &= ~(1 << UART_EN);
	
	/* Set baudrate 115200 */
	uptr->brr = UART_BAUDRATE(100 MHZ, 115200);
	
	uptr->cr1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
	uptr->cr1 |= (1 << UART_EN);
    
    #if 0
    struct uart *uartptr=&uarttab[0];
    uartptr->csr = (void *)(0x40011400);
    /* Initialize statistical counts */
    uartptr->cout = 0;
    uartptr->cin = 0;
    uartptr->lserr = 0;
    uartptr->ovrrn = 0;
    uartptr->iirq = 0;
    uartptr->oirq = 0;

    /* Initialize input buffer */
    sem_init(&uartptr->isema,0);//uartptr->isema = semcreate(0);
    uartptr->iflags = 0;
    uartptr->istart = 0;
    uartptr->icount = 0;

    /* Initialize output buffer */
    sem_init(&uartptr->osema,UART_OBLEN);//uartptr->osema = semcreate(UART_OBLEN);
    uartptr->oflags = 0;
    uartptr->ostart = 0;
    uartptr->ocount = 0;
    uartptr->oidle = 1;
    #endif

    
    NVIC_Type *np = NVIC_BASE;
	np->ISER[USART6_IRQn/32] = 1 << (USART6_IRQn%32);

}


 
void init_mem(){

#if MALLOC_MODE == 1
kprintf("\nmalloc mode 1\n");
malloc_init(KMALLOC_START, KMALLOC_LENGTH);
malloc_debug();
testmem();
#elif MALLOC_MODE == 2
kprintf("malloc mode 2\n");
meminit();
testmem();
#elif MALLOC_MODE == 3
kprintf("malloc mode 3\n");
chunklist_init(KMALLOC_LENGTH);
info();
#endif

kprintf("%10d bytes of Xinu code.\n",
	(uint32)&etext - (uint32)&text);
kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)&text, (uint32)&etext - 1);
kprintf("%10d bytes of data.\n",
	(uint32)&ebss - (uint32)&data);
kprintf("           [0x%08X to 0x%08X]\n\n",
	(uint32)&data, (uint32)&ebss - 1);

}

void mount_fs(){
FATFS FatFs;
FRESULT res;
DIR dirs;
FILINFO Finfo;


if ((res = f_mount(&FatFs, "", 1)) != FR_OK) {
    kprintf("mount error! res=%d\n", res);
    return;//return 1;
}
kprintf("mount successfully\n");



char p2[100];// = malloc(100);
strcpy(p2,"/");
if ((res = f_opendir(&dirs,p2)) == FR_OK) {
     for (;;){
         res = f_readdir(&dirs, &Finfo);
         if(res != FR_OK || Finfo.fname[0] == 0)break;
         kprintf("%s\n", Finfo.fname);
     }
 }else{
    kprintf("error dir\n");
 }
//free(p2);
}