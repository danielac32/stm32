#include <xinu.h>

devcall	spigetc (struct dentry *devptr)
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
    

    while (!(spiptr->sr & SPI_SR_TXE));
    spiptr->dr = 0xff;
    while (!(spiptr->sr & SPI_SR_RXNE));
    return (devcall)spiptr->dr;
}
