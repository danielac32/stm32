#include <xinu.h>

devcall	spiputc (struct dentry *devptr, uint8 c)
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

	/* Check if buffer is empty -- busy wait TODO: interrupts */
	while (!(spiptr->sr & SPI_SR_TXE));
	spiptr->dr = c;

	return OK;
}
