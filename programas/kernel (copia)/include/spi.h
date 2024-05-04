#include "rp2040.h"
#include "kernel.h"

#define spi 0


void spi_init(uint32,uint32,uint32);
unsigned char transfer(unsigned char);
unsigned char transfer2(unsigned char);

//void      spi_init(void);
void      spi_cs(uint32_t value);
uint8_t   spi_sendrecv(uint8_t ch);
void      spi_readblock(uint8_t *ptr, int length);
void      spi_writeblock(uint8_t *ptr, int length);