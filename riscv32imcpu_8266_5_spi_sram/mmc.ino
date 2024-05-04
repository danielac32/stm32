#include "mmc.h"

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
  uint16_t btr      /* Byte count (must be multiple of 4) */
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
  while (--btr);
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
    while (--i);
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
      if (!xmit_datablock(0, 0xFD)) count = 1; /* STOP_TRAN token */
    }
  }
  deselect();

  return count ? RES_ERROR : RES_OK;
}



byte disk_ioctl (
  byte drv,   /* Physical drive nmuber (0..) */
  byte ctrl,    /* Control code */
  void *buff    /* Buffer to send/receive control data */
)
{
  byte res;

  switch(ctrl)
  {
    case CTRL_SYNC:
      res = RES_OK;
      break;
    case GET_SECTOR_COUNT:
      *(uint32_t *)buff = 4194304; /* 2GB SD card */
      res = RES_OK;
      break;
     case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
             *(uint16_t*)buff = 512;
             res = RES_OK;
             break;
    default:
      res = RES_PARERR;
            break;
  }

  return res;
}

byte disk_status (
  byte pdrv   /* Physical drive nmuber (0) */
)
{
  if (pdrv) return STA_NOINIT;  /* Supports only single drive */
  return Stat;
}
