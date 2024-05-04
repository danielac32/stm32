/* kprintf.c -  kputc, kgetc, kprintf */
/* Kprintf uses a direct method to write to serial
 * Used for debugging purposes. Should in normal cases
 * use printf that does a syscall
 * */

#include <xinu.h>
#include <stdarg.h>

/*------------------------------------------------------------------------
 * kputc - use polled I/O to write a character to the console serial line
 *------------------------------------------------------------------------
 */
uint32 kputc(
	  byte	c			/* character to write		*/
	)
{
	uint32 mask;
	volatile struct uart_csreg * uptr = (struct uart_csreg *)0x40011400;
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
uint32 kgetc(void)
{
	// Not used
  return OK;
}

extern	void	_doprnt(char *, va_list, int (*)(int));

/*------------------------------------------------------------------------
 * kprintf  -  use polled I/O to print formatted output on the console
 *------------------------------------------------------------------------
 */


uint32 kprintf(const char *fmt, ...)
{
    va_list ap;
    //sem_wait(&uart_tx);
    va_start(ap, fmt);
    _doprnt((char *)fmt, ap, kputc);
    va_end(ap);
    //sem_signal(&uart_tx);
    return OK;
}



int32 printf(const char *fmt, ...){
   va_list ap;
    sem_wait(&uart_tx);
    va_start(ap, fmt);
    _doprnt((char *)fmt, ap, syscall_putc);
    va_end(ap);
    sem_signal(&uart_tx);
    return OK;
}

void hexDump2(uint32 offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

void hexDump(uint32 offset,char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%x (%s):\n", addr, desc);
    else
        printf("%x:\n", addr);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}
