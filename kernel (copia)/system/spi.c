/*
 * spi.c --- STM32F4 SPI driver.
 *
 * Copyright (C) 2012, Galois, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include <xinu.h>

/*
 * Utility macros to make busy waits obvious.  These could be moved to
 * a general utility header.
 */




//struct spi_bus _spi1 = {
  //SPI1,                         /* dev */
 // RCCDEV_SPI1,                  /* rcc_dev */
 // PIN_AF_SPI1,                  /* af */
 // pin_a7,                       /* mosi_pin */
 // pin_a6,                       /* miso_pin */
 // pin_a5,                       /* sck_pin */
// };

//struct spi_bus _spi2 = {
// SPI2,                         /* dev */
 // RCCDEV_SPI2,                  /* rcc_dev */
 // PIN_AF_SPI2,                  /* af */
 // pin_c3,                       /* mosi_pin */
 // pin_c2,                       /* miso_pin */
 // pin_b10,                      /* sck_pin */
//};

//struct spi_bus _spi3 = {
  //SPI3,                         /* dev */
  //RCCDEV_SPI3,                  /* rcc_dev */
  //PIN_AF_SPI3,                  /* af */
  //pin_c12,                      /* mosi_pin */
  //pin_c11,                      /* miso_pin */
  //pin_c10,                      /* sck_pin */
//};


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