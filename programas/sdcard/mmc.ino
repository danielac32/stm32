#include "integer.h"
#include <Wire.h>
#include <PCF8574.h>
PCF8574 pcf(56);
#define nok_cs(x)  pcf.write(0, x==1?HIGH:LOW);//digitalWrite(D0,x)
#define nok_dc(x)  pcf.write(1, x==1?HIGH:LOW);//digitalWrite(D1,x)

#define CS_RAM1  2
#define CS_RAM2  0
#define MMC_Chip_Select  10    
#define MMC_Disable() digitalWrite(MMC_Chip_Select,true)
#define MMC_Enable() digitalWrite(MMC_Chip_Select,false)

unsigned char mmc_init (){
  unsigned int Timeout = 0;
    SPI.begin();
    
    pinMode(CS_RAM1,OUTPUT);
    pinMode(CS_RAM2,OUTPUT);
    digitalWrite(CS_RAM1,true);
    digitalWrite(CS_RAM2,true);
    
    Wire.begin();
    pcf.begin();
    nok_cs(1);
    
    pinMode(MMC_Chip_Select,OUTPUT);
    MMC_Disable(); 
    
  for (unsigned char b = 0;b<10;b++){
       mmc_writebyte(0xff);
  }
  
  unsigned char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
  while(mmc_write_command (CMD) !=1){
    if (Timeout++ > 200){
        MMC_Disable();
        return(1); 
    }
  }
  Timeout = 0;
  CMD[0] = 0x41;//Commando 1
  CMD[5] = 0xFF;
  while(mmc_write_command (CMD) !=0){
        if (Timeout++ > 400){
      MMC_Disable();
      return(2); //Abbruch bei Commando2 (Return Code2)
        }
  }
  MMC_Disable();
  return(0);
}

unsigned char mmc_write_command (unsigned char *cmd){
  unsigned char tmp = 0xff;
  unsigned int Timeout = 0;
  MMC_Disable();
  mmc_writebyte(0xFF);
  MMC_Enable();
  for (unsigned char a = 0;a<0x06;a++){
       mmc_writebyte(*cmd++);
  }

  while (tmp == 0xff){
         tmp = mmc_readbyte();
         if (Timeout++ > 500){
             break;
         }
  }
        MMC_Disable();
  return(tmp);
}


unsigned char mmc_readbyte (void){
//  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  unsigned char c = SPI.transfer(0xff);
//  SPI.endTransaction();
  return c;
}
void mmc_writebyte (unsigned char Byte){
//  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(Byte);
//  SPI.endTransaction();
}


void mmc_read_block(unsigned char *cmd,unsigned char *Buffer,unsigned int Bytes){ 
        MMC_Enable();
  if (mmc_write_command (cmd) != 0){
      return;
  }
  
  while (mmc_readbyte() != 0xfe){};
  for (unsigned int a=0;a<Bytes;a++){
       *Buffer++ = mmc_readbyte();
  }
  mmc_readbyte();//CRC - Byte wird nicht ausgewertet
  mmc_readbyte();//CRC - Byte wird nicht ausgewertet
  MMC_Disable();
  return;
}


unsigned char mmc_read_sector (unsigned long addr,unsigned char *Buffer){ 
  unsigned char cmd[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
  addr = addr << 9; //addr = addr * 512
  cmd[1] = ((addr & 0xFF000000) >>24 );
  cmd[2] = ((addr & 0x00FF0000) >>16 );
  cmd[3] = ((addr & 0x0000FF00) >>8 );
  mmc_read_block(cmd,Buffer,512);
  return(0);
}

unsigned char mmc_write_sector (unsigned long addr,unsigned char *Buffer)
{
  unsigned char cmd[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 
  addr = addr << 9; //addr = addr * 512
  
  cmd[1] = ((addr & 0xFF000000) >>24 );
  cmd[2] = ((addr & 0x00FF0000) >>16 );
  cmd[3] = ((addr & 0x0000FF00) >>8 );
  mmc_write_block(cmd,Buffer,512);
  
  return 0;
}

void mmc_write_block(unsigned char *cmd,unsigned char *Buffer,unsigned int Bytes){ 
 
  if (mmc_write_command (cmd) != 0){
      return;
  }
   MMC_Enable();
for (unsigned char a=0;a<100;a++){
    mmc_readbyte();
}
  
  mmc_writebyte(0xFE); 
  
  for (unsigned int a=0;a<512;a++)
    {
    mmc_writebyte(*Buffer++);
    }
  
  mmc_writebyte(0xFF); 
  mmc_writebyte(0xFF); 
  MMC_Disable();
  return;
}


unsigned char mmc_write_sector2 (unsigned long addr,unsigned char *Buffer)
{
  unsigned char tmp;
  unsigned char cmd[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 

  addr = addr << 9; //addr = addr * 512
  
  cmd[1] = ((addr & 0xFF000000) >>24 );
  cmd[2] = ((addr & 0x00FF0000) >>16 );
  cmd[3] = ((addr & 0x0000FF00) >>8 );

  tmp = mmc_write_command (cmd);
  if (tmp != 0)
    {
    return(tmp);
    }
      
  for (unsigned char a=0;a<100;a++)
    {
    mmc_readbyte();
    }
  
  mmc_writebyte(0xFE); 
  
  for (unsigned int a=0;a<512;a++)
    {
    mmc_writebyte(*Buffer++);
    }
  
  mmc_writebyte(0xFF); 
  mmc_writebyte(0xFF); 
  
  if((mmc_readbyte()&0x1F) != 0x05) return(1);

 
  while (mmc_readbyte() != 0xff){};
  
 
  MMC_Disable();
  
return(0);
}


/****************************************************************************************************/



typedef byte  DSTATUS;

/* Results of Disk Functions */
typedef enum {
  RES_OK = 0,   /* 0: Successful */
  RES_ERROR,    /* 1: R/W Error */
  RES_WRPRT,    /* 2: Write Protected */
  RES_NOTRDY,   /* 3: Not Ready */
  RES_PARERR    /* 4: Invalid Parameter */
} DRESULT;



/* Definitions for MMC/SDC command */
#define CMD0  (0)     /* GO_IDLE_STATE */
#define CMD1  (1)     /* SEND_OP_COND (MMC) */
#define ACMD41  (0x80+41) /* SEND_OP_COND (SDC) */
#define CMD8  (8)     /* SEND_IF_COND */
#define CMD9  (9)     /* SEND_CSD */
#define CMD10 (10)    /* SEND_CID */
#define CMD12 (12)    /* STOP_TRANSMISSION */
#define ACMD13  (0x80+13) /* SD_STATUS (SDC) */
#define CMD16 (16)    /* SET_BLOCKLEN */
#define CMD17 (17)    /* READ_SINGLE_BLOCK */
#define CMD18 (18)    /* READ_MULTIPLE_BLOCK */
#define CMD23 (23)    /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80+23) /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24 (24)    /* WRITE_BLOCK */
#define CMD25 (25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD32 (32)    /* ERASE_ER_BLK_START */
#define CMD33 (33)    /* ERASE_ER_BLK_END */
#define CMD38 (38)    /* ERASE */
#define CMD55 (55)    /* APP_CMD */
#define CMD58 (58)    /* READ_OCR */


/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC    0x01    /* MMC ver 3 */
#define CT_SD1    0x02    /* SD ver 1 */
#define CT_SD2    0x04    /* SD ver 2 */
#define CT_SDC    (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK  0x08    /* Block addressing */

#define STA_NOINIT    0x01  /* Drive not initialized */
#define STA_NODISK    0x02  /* No medium in the drive */
#define STA_PROTECT   0x04  /* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC     0 /* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT  1 /* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE   2 /* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE    3 /* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM     4 /* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER      5 /* Get/Set power status */
#define CTRL_LOCK     6 /* Lock/Unlock media removal */
#define CTRL_EJECT      7 /* Eject media */
#define CTRL_FORMAT     8 /* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE    10  /* Get card type */
#define MMC_GET_CSD     11  /* Get CSD */
#define MMC_GET_CID     12  /* Get CID */
#define MMC_GET_OCR     13  /* Get OCR */
#define MMC_GET_SDSTAT    14  /* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV     20  /* Get F/W revision */
#define ATA_GET_MODEL   21  /* Get model name */
#define ATA_GET_SN      22  /* Get serial number */

static
DSTATUS Stat = STA_NOINIT;  /* Disk status */

static
byte CardType;      /* Card type flags */


#define sd_init()  SPI.begin();
#define sd_open() 0//SPI2_Open()
#define sd_tx(d)  SPI.transfer(d)
#define sd_rx()   SPI.transfer(0xff)


byte wait_ready (void)  /* 1:Ready, 0:Timeout */
{
  uint16_t tmr;


  for (tmr = 5000; tmr; tmr--) {  /* Wait for ready in timeout of 500ms */
    if (sd_rx() == 0xFF) break;
    delayMicroseconds(100);
  }

  return tmr ? 1 : 0;
}

void deselect (void)
{
  MMC_Disable();
  sd_rx();  /* Dummy clock (force DO hi-z for multiple slave SPI) */
}

byte select (void)  /* 1:Successful, 0:Timeout */
{
   MMC_Enable();
   sd_rx();  /* Dummy clock (force DO enabled) */
   if (wait_ready()) return 1; /* Wait for card ready */

   deselect();
   return 0; /* Timeout */
}


byte rcvr_datablock (
  byte *buff,     /* Data buffer to store received data */
  uint8_t btr      /* Byte count (must be multiple of 4) */
)
{
  byte token;
  uint16_t tmr;
  uint16_t block=512;

  for (tmr = 2000; tmr; tmr--) {  /* Wait for data packet in timeout of 200ms */
    token = sd_rx();
    if (token != 0xFF) break;
    delayMicroseconds(100);
  }
  if (token != 0xFE) return 0;  /* If not valid data token, retutn with error */

  do
    *buff++ = sd_rx();    /* Receive the data block into buffer */
  while (--block);
  sd_rx();          /* Discard CRC */
  sd_rx();

  return 1;         /* Return with success */
}



byte xmit_datablock (
  byte *buff, /* 512 byte data block to be transmitted */
  byte token      /* Data/Stop token */
)
{
  byte resp;
  uint16_t i;
  uint16_t block=512;

  if (!wait_ready()) return 0;

  sd_tx(token);     /* Xmit data token */
  if (token != 0xFD) {  /* Is data token */
    i = 512;
    do
      sd_tx(*buff++);       /* Xmit the data block  the MMC */
    while (--block);
    sd_rx();            /* CRC (Dummy) */
    sd_rx();
    resp = sd_rx();         /* Reveive data response */
    if ((resp & 0x1F) != 0x05)    /* If not accepted, return with error */
      return 0;
  }

  return 1;
}




byte __send_cmd (    /* Returns R1 resp (bit7==1:Send failed) */
  byte cmd,   /* Command index */
  uint32_t arg   /* Argument */
)
{
  byte n, res;


  /* Select the card and wait for ready except to stop multiple block read */
  if (cmd != CMD12) {
    deselect();
    if (!select()) return 0xFF;
  }

  /* Send command packet */
  sd_tx(0x40 | cmd);        /* Start + Command index */
  sd_tx((BYTE)(arg >> 24));   /* Argument[31..24] */
  sd_tx((BYTE)(arg >> 16));   /* Argument[23..16] */
  sd_tx((BYTE)(arg >> 8));    /* Argument[15..8] */
  sd_tx((BYTE)arg);       /* Argument[7..0] */
  n = 0x01;           /* Dummy CRC + Stop */
  if (cmd == CMD0) n = 0x95;    /* Valid CRC for CMD0(0) + Stop */
  if (cmd == CMD8) n = 0x87;    /* Valid CRC for CMD8(0x1AA) Stop */
  sd_tx(n);

  /* Receive command response */
  if (cmd == CMD12) sd_rx();    /* Skip a stuff byte when stop reading */
  n = 10;               /* Wait for a valid response in timeout of 10 attempts */
  do
    res = sd_rx();
  while ((res & 0x80) && --n);

  return res;     /* Return with the response value */
}



byte send_cmd (    /* Returns R1 resp (bit7==1:Send failed) */
  byte cmd,   /* Command index */
  uint32_t arg   /* Argument */
)
{
  byte res;

  if (cmd & 0x80) { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
      cmd &= 0x7F;
      res = __send_cmd(CMD55, 0);
      if (res > 1) return res;
  }
  return __send_cmd(cmd, arg);  /* Return with the response value */
}



byte disk_initialize (
  byte pdrv   /* Physical drive nmuber (0) */
)
{
  byte n, cmd, ty, ocr[4];
  uint16_t tmr;


  if (pdrv) return STA_NOINIT;    /* Supports only single drive */

  if (Stat & STA_NODISK) return Stat; /* No card in the socket */

  sd_init();

  pinMode(CS_RAM1,OUTPUT);
    pinMode(CS_RAM2,OUTPUT);
    digitalWrite(CS_RAM1,true);
    digitalWrite(CS_RAM2,true);
    
    Wire.begin();
    pcf.begin();
    nok_cs(1);
    
    pinMode(MMC_Chip_Select,OUTPUT);
    MMC_Disable(); 
    
  for (n = 10; n; n--) sd_rx(); /* 80 dummy clocks */

  ty = 0;
  if (send_cmd(CMD0, 0) == 1) {     /* Enter Idle state */
    if (send_cmd(CMD8, 0x1AA) == 1) { /* SDv2? */
      for (n = 0; n < 4; n++) ocr[n] = sd_rx(); /* Get trailing return value of R7 resp */
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) {   /* The card can work at vdd range of 2.7-3.6V */
        for (tmr = 1000; tmr; tmr--) {      /* Wait for leaving idle state (ACMD41 with HCS bit) */
          if (send_cmd(ACMD41, 1UL << 30) == 0) break;
          delay(1);
        }
        if (tmr && send_cmd(CMD58, 0) == 0) {   /* Check CCS bit in the OCR */
          for (n = 0; n < 4; n++) ocr[n] = sd_rx();
          ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;  /* SDv2 */
        }
      }
    } else {              /* SDv1 or MMCv3 */
      if (send_cmd(ACMD41, 0) <= 1)   {
        ty = CT_SD1; cmd = ACMD41;  /* SDv1 */
      } else {
        ty = CT_MMC; cmd = CMD1;  /* MMCv3 */
      }
      for (tmr = 1000; tmr; tmr--) {      /* Wait for leaving idle state */
        if (send_cmd(cmd, 0) == 0) break;
        delay(1);
      }
      if (!tmr || send_cmd(CMD16, 512) != 0)  /* Set R/W block length to 512 */
        ty = 0;
    }
  }
  CardType = ty;
  deselect();

  if (ty) {     /* Initialization succeded */
    Stat &= ~STA_NOINIT;    /* Clear STA_NOINIT */
    sd_open();
  }

  return Stat;
}



byte disk_read (
  byte pdrv,      /* Physical drive nmuber (0) */
  byte *buff,     /* Pointer to the data buffer to store read data */
  uint32_t sector,   /* Start sector number (LBA) */
  uint16_t count      /* Sector count (1..128) */
)
{
  byte cmd;


  if (pdrv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;

  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

  cmd = count > 1 ? CMD18 : CMD17;      /*  READ_MULTIPLE_BLOCK : READ_SINGLE_BLOCK */
  if (send_cmd(cmd, sector) == 0) {
    do {
      if (!rcvr_datablock(buff, 512)) break;
      buff += 512;
    } while (--count);
    //rcvr_datablock(buff, 512);
    
    if (cmd == CMD18) send_cmd(CMD12, 0); /* STOP_TRANSMISSION */
  }
  deselect();

  return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/


byte disk_write (
  byte pdrv,      /* Physical drive nmuber (0) */
  byte *buff, /* Pointer to the data to be written */
  uint32_t sector,   /* Start sector number (LBA) */
  uint16_t count      /* Sector count (1..128) */
)
{
  if (pdrv || !count) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  if (Stat & STA_PROTECT) return RES_WRPRT;

  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

  if (count == 1) { /* Single block write */
    if ((send_cmd(CMD24, sector) == 0)  /* WRITE_BLOCK */
      && xmit_datablock(buff, 0xFE))
      count = 0;
  }
  else {        /* Multiple block write */
    if (CardType & CT_SDC) send_cmd(ACMD23, count);
    if (send_cmd(CMD25, sector) == 0) { /* WRITE_MULTIPLE_BLOCK */
      do {
        if (!xmit_datablock(buff, 0xFC)) break;
        buff += 512;
      } while (--count);
      //xmit_datablock(buff, 0xFC);
      
      if (!xmit_datablock(0, 0xFD)) /* STOP_TRAN token */
        count = 1;
    }
  }
  deselect();

  return count ? RES_ERROR : RES_OK;
}
