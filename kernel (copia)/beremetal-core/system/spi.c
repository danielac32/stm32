#include <xinu.h>


int spi_init(){
	struct spi_csreg *spiptr;
	spiptr = (struct spi_csreg *)SPI1_BASE;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	spiptr->cr1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_TRANSFER_CLK_DIV | SPI_CR1_MSTR;//spiptr->cr1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_INIT_CLK_DIV | SPI_CR1_MSTR;
	return OK;
}

uint8 transfer(uint8 c){
	struct spi_csreg *spiptr;
	spiptr = (struct spi_csreg *)SPI1_BASE;

	while (!(spiptr->sr & SPI_SR_TXE));
	spiptr->dr = c;
	while (!(spiptr->sr & SPI_SR_RXNE));
	return spiptr->dr;
}