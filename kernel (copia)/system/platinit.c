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

/* platinit.c - platinit */

#include <xinu.h>

void uartinit();

/*------------------------------------------------------------------------
 * platinit - platform specific initialization
 *------------------------------------------------------------------------
 */
void platinit(void)
{
	/* Load .data segment into SRAM */
	//extern uint32 * etext, data, edata;
	//int size = (&edata - &data) * 4;
	//memcpy(&data, &etext, size);	
    

    //SystemInit();
	/* Initialize UART Serial communication */
	uartinit();
	
	/* Initialize the Interrupt Controller (evec.c) */
	//initintc();

	/* System calls */
	//syscall_init();
}

/* Simple UART initialization, not using a TTY device
Used by kprintf and early system debugging */

void uartinit()
{
	
	struct	uart_csreg *uptr;	/* Address of UART's CSRs */
	//struct 	gpio_csreg *gptr;	
	//struct 	clock_csreg *cptr;
    

    init_sys();         /* Set MCU speed, enable preipherials, etc. */
   // init_gpio();        /* Configure ports and pins                 */
    //init_usart(115200); /* Configure USART6 for 115200 baud         */


	/* Enable 'clock' on peripherals */
	//cptr = (struct clock_csreg *)CLOCK_BASE;
	//cptr->apb2enr |= (1 << IOPAEN) | (1 << USART1EN);

	/* Set in and output mode (dirty) */
	//gptr = (struct gpio_csreg *)(0x40010800);
	//gptr->crh = 0x44444894;


    hw_cfg_pin(GPIOx(GPIO_A),     11,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),     12,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	/* Enable peripheral */
	uptr = (struct uart_csreg *)(0x40011400);
	uptr->cr1 &= ~(1 << UART_EN);
	
	/* Set baudrate 115200 */
	uptr->brr = UART_BAUDRATE(100 MHZ, 115200);
	
	uptr->cr1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
	uptr->cr1 |= (1 << UART_EN);
	
	
}
