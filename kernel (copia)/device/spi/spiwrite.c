#include <xinu.h>

devcall	spiwrite (struct dentry *devptr,uint8 c)
{
	struct spi_csreg *spiptr;	/* SPI control and status regs	*/

	/* Configure SPI peripheral */
	if(devptr->dvminor == 0) {
		spiptr = (struct spi_csreg *)SPI1_BASE;
	}
	else {
		kprintf("SPI device not recognized\n");
		return SYSERR;
	}
 

	return OK;
}
