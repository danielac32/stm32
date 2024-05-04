
#include "diskio.h"//"w25qxxx.h"

 #define FLASH_SECTOR_SIZE  4096       
//对于W25Q64 
//前6M字节给fatfs用,6M字节后~6M+500K给用户用,6M+500K以后,用于存放字库,字库占用1.5M.             
uint16_t     FLASH_SECTOR_COUNT=2048*6;//6M字节,默认为W25Q64
#define FLASH_BLOCK_SIZE    8     //每个BLOCK有8个扇区

uint32_t disk_read_correct_count=0;
uint32_t disk_read_error_count=0;
uint32_t disk_write_correct_count=0;
uint32_t disk_write_error_count=0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
  DSTATUS stat=0;
  
  
      

  return stat;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS stat = RES_OK;

  SPI_Flash_Init();
  if(SPI_FLASH_TYPE==W25Q64)
    FLASH_SECTOR_COUNT=2048*6;//W25Q64
  else 
    FLASH_SECTOR_COUNT=2048*2;

  return stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DRESULT res;

  for(;count>0;count--)
  {
    SPI_Flash_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
    sector++;
    buff+=FLASH_SECTOR_SIZE;
  }
  res=0; 

  return res;
}

 
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
  DRESULT res;

  for(;count>0;count--)
  {                       
     SPI_Flash_Write((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
     sector++;
     buff+=FLASH_SECTOR_SIZE;
  }
  res=0; 
  return res;
}
 
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res=0;
  switch(cmd)
      {
  case CTRL_SYNC:
        res = RES_OK; 
            break;   
        case GET_SECTOR_SIZE:
            *(WORD*)buff = FLASH_SECTOR_SIZE;
            res = RES_OK;
            break;   
        case GET_BLOCK_SIZE:
            *(WORD*)buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;   
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = FLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
 }
  return res;
}
 
 DWORD get_fattime (void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
