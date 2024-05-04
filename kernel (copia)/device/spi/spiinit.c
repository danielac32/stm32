#include <xinu.h>

int32	spiinit (
		 struct	dentry *devptr
		)
{
	struct spi_csreg *spiptr;	/* SPI control and status regs	*/
	//struct clock_csreg *cptr; 

	//kprintf("INIT SPI!\n");


	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 

	/* Configure SPI peripheral */
	if(devptr->dvminor == 0) {
		spiptr = (struct spi_csreg *)SPI1_BASE;//(devptr->dvcsr);
	}
	else {
		kprintf("SPI device not recognized\n");
		return SYSERR;
	}

	spiptr->cr1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_INIT_CLK_DIV | SPI_CR1_MSTR;


	return OK;
}
