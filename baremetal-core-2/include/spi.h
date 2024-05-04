/* spi.h - SPI definitions */



#define SPI_INIT_CLK_DIV          (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
// SPI Data Transfer Frequency (25MHz max)
// SPI1_CK = PCLK2(84MHz) / 4 = 21MHz
#define SPI_TRANSFER_CLK_DIV      SPI_CR1_BR_0


struct	spi_csreg {
	volatile uint32 cr1;		
	volatile uint32 cr2;		
	volatile uint32 sr;	
	volatile uint32 dr;
	volatile uint32 crcpr;		
	volatile uint32 rxcrcr;
	volatile uint32 txcrcr;		
	volatile uint32 i2scfgr;
	volatile uint32 i2spr;	
};

/* No SPI0 peripheral, starts from 1*/
//#define	SPI1_BASE	0x40013000

/* Control register settings */
#define SPI_MSTR	2
#define SPI_ENABLE	6
#define SPI_SSM		9
#define	SPI_SSI		8
#define	SPI_BIDIMODE	15
#define SPI_BIDIOE	14

/* Status register flags */
#define SPI_BSY		7 // Busy
#define SPI_TXE		1 // Transfer buffer empty
#define SPI_RXNE	2 // Receive buffer NOT empty

/* Baud rate control */
#define BR_FPCLKDIV2	0x00000000
#define BR_FPCLKDIV4	0x00000008
#define	BR_FPCLKDIV8	0x00000010
#define BR_FPCLKDIV16	0x00000018
#define	BR_FPCLKDIV32	0x00000020
#define BR_FPCLKDIV64	0x00000028

	 
enum spi_baud {
  SPI_BAUD_DIV_2   = 0x00,
  SPI_BAUD_DIV_4   = 0x01,
  SPI_BAUD_DIV_8   = 0x02,
  SPI_BAUD_DIV_16  = 0x03,
  SPI_BAUD_DIV_32  = 0x04,
  SPI_BAUD_DIV_64  = 0x05,
  SPI_BAUD_DIV_128 = 0x06,
  SPI_BAUD_DIV_256 = 0x07
};


extern int spi_init();
extern uint8 transfer(uint8 );
extern void      spi_cs(uint32 value);
extern uint8_t   spi_sendrecv(uint8 ch);
extern void      spi_readblock(uint8 *ptr, int length);
extern void      spi_writeblock(uint8 *ptr, int length);
 
