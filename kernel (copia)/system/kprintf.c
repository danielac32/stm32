/* kprintf.c -  kputc, kgetc, kprintf */
/* Kprintf uses a direct method to write to serial
 * Used for debugging purposes. Should in normal cases
 * use printf that does a syscall
 * */

#include <xinu.h>
#include <stdarg.h>
#include <syscall.h>

/*------------------------------------------------------------------------
 * kputc - use polled I/O to write a character to the console serial line
 *------------------------------------------------------------------------
 */
syscall kputc(
	  byte	c			/* character to write		*/
	)
{
	intmask mask;
	volatile struct uart_csreg * uptr = 0x40011400;
	mask = disable();

	if (c == '\n') {
              while(!(uptr->sr & UART_TC));
	     uptr->dr = 0x0D; // return line
	    //USART6->DR = 0x0D;
        //while(!(USART6->SR &  USART_SR_TXE));
      	}
              while(!(uptr->sr & UART_TC));
	          uptr->dr = c;
        //USART6->DR = c;
        //while(!(USART6->SR &  USART_SR_TXE));


	restore(mask);
	return OK;
}

/*------------------------------------------------------------------------
 * kgetc - use polled I/O to read a character from the console serial line
 *------------------------------------------------------------------------
 */
syscall kgetc(void)
{
	// Not used
}

extern	void	_doprnt(char *, va_list, int (*)(int));

/*------------------------------------------------------------------------
 * kprintf  -  use polled I/O to print formatted output on the console
 *------------------------------------------------------------------------
 */

static int __kputc(int c){
     __syscall5(0,c);
     return OK;
}

syscall kprintf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _doprnt(fmt, ap, (int (*)(int))kputc);
    va_end(ap);
    return OK;
}


