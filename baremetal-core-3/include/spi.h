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
extern int spi2_init();
extern uint8 transfer(uint8 );
extern uint8 transfer2(uint8 );
extern void      spi_cs(uint32 value);
extern uint8_t   spi_sendrecv(uint8 ch);
extern void      spi_readblock(uint8 *ptr, int length);
extern void      spi_writeblock(uint8 *ptr, int length);
 

#define CS_RAM1  1
#define CS_RAM2  2


#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2

#define SPIFLASH_WRITEENABLE      0x06        // write enable
#define SPIFLASH_WRITEDISABLE     0x04        // write disable

#define SPIFLASH_BLOCKERASE_4K    0x20        // erase one 4K block of flash memory
#define SPIFLASH_BLOCKERASE_32K   0x52        // erase one 32K block of flash memory
#define SPIFLASH_BLOCKERASE_64K   0xD8        // erase one 64K block of flash memory
#define SPIFLASH_CHIPERASE        0x60        // chip erase (may take several seconds depending on size)
                                              // but no actual need to wait for completion (instead need to check the status register BUSY bit)
#define SPIFLASH_STATUSREAD       0x05        // read status register
#define SPIFLASH_STATUSWRITE      0x01        // write status register
#define SPIFLASH_ARRAYREAD        0x0B        // read array (fast, need to add 1 dummy byte after 3 address bytes)
#define SPIFLASH_ARRAYREADLOWFREQ 0x03        // read array (low frequency)

#define SPIFLASH_SLEEP            0xB9        // deep power down
#define SPIFLASH_WAKE             0xAB        // deep power wake up
#define SPIFLASH_BYTEPAGEPROGRAM  0x02        // write (1 to 256bytes)
#define SPIFLASH_IDREAD           0x9F        // read JEDEC manufacturer and device ID (2 bytes, specific bytes for each manufacturer and device)
                                              // Example for Atmel-Adesto 4Mbit AT25DF041A: 0x1F44 (page 27: http://www.adestotech.com/sites/default/files/datasheets/doc3668.pdf)
                                              // Example for Winbond 4Mbit W25X40CL: 0xEF30 (page 14: http://www.winbond.com/NR/rdonlyres/6E25084C-0BFE-4B25-903D-AE10221A0929/0/W25X40CL.pdf)
#define SPIFLASH_MACREAD          0x4B        // read unique ID number (MAC)


#define RAM_HALF 128000

uint8 readByte(uint8 cs,uint32 addr);
void writeByte(uint8 cs,uint32 addr, uint8 byt);
void command(uint8 cmd, bool isWrite);
bool busy(uint8 cs);
uint8 readStatus(uint8 cs);

void readBytes(uint8 cs,uint32 addr, void* buf, uint16 len);
void writeBytes(uint8 cs,uint32 addr, const void* buf, uint16 len);
void accessPSRAM(uint32 addr, size_t size, bool write, void *bufP);
