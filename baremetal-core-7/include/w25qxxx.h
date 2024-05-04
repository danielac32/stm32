/*
 * w25qxxx.h
 *
 *  Created on: Feb 13, 2022
 *      Author: Maxiufeng
 */

/*
 * 			W25Q64	 64M-bit ->  8M-Byte
 * 			W25Q128	128M-bit -> 16M-Byte
 * 			W25Q256	256M-bit -> 32M-Byte
 */

/**
  * Use polling to transmit SPI data
  */

/**
  * @brief Implement w25qxxx low-level operations using standard spi and HAL libraries
  *		Provides a set of interface functions for diskio.c through a function pointer structure
  *		 If using FATFs, the following #define is set to 1
  * 	   #define EN_SPI_FLASH_DISK_IO 	0	//1:enable spi_flash_disk_io 0:disable
  */

#ifndef INC_W25QXXX_H_
#define INC_W25QXXX_H_

#include <stdint.h>
#include <stm32.h>
#include <gpio.h>
#include <kernel.h>
typedef BYTE  DSTATUS;


/* Results of Disk Functions */
typedef enum {
  RES_OK = 0,   /* 0: Successful */
  RES_ERROR,    /* 1: R/W Error */
  RES_WRPRT,    /* 2: Write Protected */
  RES_NOTRDY,   /* 3: Not Ready */
  RES_PARERR    /* 4: Invalid Parameter */
} DRESULT;

/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT    0x01  /* Drive not initialized */
#define STA_NODISK    0x02  /* No medium in the drive */
#define STA_PROTECT   0x04  /* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC     0 /* Complete pending write process (needed at FF_FS_READONLY == 0) */
#define GET_SECTOR_COUNT  1 /* Get media size (needed at FF_USE_MKFS == 1) */
#define GET_SECTOR_SIZE   2 /* Get sector size (needed at FF_MAX_SS != FF_MIN_SS) */
#define GET_BLOCK_SIZE    3 /* Get erase block size (needed at FF_USE_MKFS == 1) */
#define CTRL_TRIM     4 /* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */

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
#define ISDIO_READ      55  /* Read data form SD iSDIO register */
#define ISDIO_WRITE     56  /* Write data to SD iSDIO register */
#define ISDIO_MRITE     57  /* Masked write data to SD iSDIO register */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV     20  /* Get F/W revision */
#define ATA_GET_MODEL   21  /* Get model name */
#define ATA_GET_SN      22  /* Get serial number */


#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

extern uint16_t SPI_FLASH_TYPE;//定义我们使用的flash芯片型号

//#define	SPI_FLASH_CS PCout(4)  //选中FLASH	
				 
////////////////////////////////////////////////////////////////////////////
 
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
uint16_t  SPI_Flash_ReadID(void);  	    //读取FLASH ID
uint8_t	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(uint8_t sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒

 

#endif /* INC_W25QXXX_H_ */