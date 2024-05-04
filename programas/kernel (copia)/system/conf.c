#include <os.h>



local void 	ioerr(void){

}
local void	ionull(void){

}

struct	dentry	devtab[NDEVS] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, //control,
 * //dev-csr-address, //intr-handler, //irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyinit, (void *)ionull, (void *)ionull,
	  (void *)ttyread, (void *)ttywrite, (void *)ioerr,
	  (void *)ioerr, (void *)ttyputc},
	  //(void *)0x40013800, (void *)ttyhandler, 53 },

/* GPIO0 is gpio */
	{ 1, 0, "GPIO",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull},
	  //(void *)0x40010800, (void *)gpiohandler, 99 },

/* NAMESPACE is nam */
	{ 3, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr},
	 // (void *)0x0, (void *)ioerr, 0 },
};

