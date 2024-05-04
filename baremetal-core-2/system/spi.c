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


//-----------------------------------------------------------------
// spi_cs: Set chip select
//-----------------------------------------------------------------
void spi_cs(uint32 value)
{
    hw_set_pin(GPIOx(GPIO_A), 4, value);
}
//-----------------------------------------------------------------
// spi_sendrecv: Send or receive a character
//-----------------------------------------------------------------
uint8 spi_sendrecv(uint8 data)
{
    struct spi_csreg *spiptr;
	spiptr = (struct spi_csreg *)SPI1_BASE;

	while (!(spiptr->sr & SPI_SR_TXE));
	spiptr->dr = data;
	while (!(spiptr->sr & SPI_SR_RXNE));
	return spiptr->dr;
    // 1. Write data to SPI Tx FIFO
    // 2. Wait for Tx complete
    // 3. Read SPI Rx FIFO and return
}
//-----------------------------------------------------------------
// spi_readblock: Read a block of data from a device
//-----------------------------------------------------------------
void spi_readblock(uint8 *ptr, int length)
{
    int i;

    for (i=0;i<length;i++)
        *ptr++ = spi_sendrecv(0xFF);
}
//-----------------------------------------------------------------
// spi_writeblock: Write a block of data to a device
//-----------------------------------------------------------------
void spi_writeblock(uint8 *ptr, int length)
{
    int i;

    for (i=0;i<length;i++)
        spi_sendrecv(*ptr++);
}
