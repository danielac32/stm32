/* ttyhandler.c - ttyhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void ttyhandler(uint32 xnum) {
	struct	dentry	*devptr;	/* Address of device control blk*/
	struct	ttycblk	*typtr;		/* Pointer to ttytab entry	*/	
	struct	uart_csreg *csrptr;	/* Address of UART's CSR	*/
//	uint32	iir = 0;		/* Interrupt identification	*/
//	uint32	lsr = 0;		/* Line status			*/
//
//
	/* Get CSR address of the device (assume console for now) */

	devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct uart_csreg *) devptr->dvcsr;

	/* Obtain a pointer to the tty control block */

	typtr = &ttytab[devptr->dvminor];

	/* Test type of UART interrupt */

	uint8 flags = csrptr->sr & 0xFF;
	//kprintf("%x", flags);	
	if (flags & UART_RXNE) {
		ttyhandle_in(typtr, csrptr);
		return;
	}
	
	else if (flags & UART_TC)  {
		ttyhandle_out(typtr, csrptr);
		return;
		
	}
}
