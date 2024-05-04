
#include "kernel.h"

#define SPI_CS_BIT      13
#define SPI_MOSI_BIT    12
#define SPI_MISO_BIT    11 //in
#define SPI_SCK_BIT     10


//############################################################################
//Routine zur Initialisierung der MMC/SD-Karte (SPI-MODE)
unsigned char mmc_init ();
  
unsigned char mmc_write_command (unsigned char *cmd);
  
unsigned char mmc_read_byte (void);
 
void mmc_write_byte (unsigned char Byte);

unsigned char mmc_write_sector (unsigned long addr,unsigned char *Buffer);
 
void mmc_read_block(unsigned char *cmd,unsigned char *Buffer,unsigned int Bytes);
 
unsigned char mmc_read_sector (unsigned long addr,unsigned char *Buffer);
 
unsigned char mmc_read_cid (unsigned char *Buffer);
 
unsigned char mmc_read_csd (unsigned char *Buffer);