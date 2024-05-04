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
//#include <stm32f4xx.h>
#include "kernel.h"

#define SPI_FLASH_SECTOR_SIZE    512      // 
#define SPI_FLASH_SECTOR_COUNT   15625    // 32768*512 = 16MB
#define SPI_FLASH_BLOCK_SIZE     4096        // 4KB


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