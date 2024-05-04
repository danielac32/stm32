/*
 * w25qxxx.c
 *
 *  Created on: Feb 13, 2022
 *      Author: Maxiufeng
 */


#include "w25qxxx.h"
#include "gpio.h"

uint16_t SPI_FLASH_TYPE=W25Q64;//默认就是25Q16

#define	PORT_SCK        GPIO_A   // PB3 --> SPI1_SCK
#define	PIN_SCK         5
#define	PORT_MISO       GPIO_A   // PB4 <-- SPI1_MISO
#define	PIN_MISO        6
#define	PORT_MOSI       GPIO_A   // PB5 --> SPI1_MOSI
#define	PIN_MOSI        7
#define	PORT_CS         GPIO_A   // PB14 --> F_CS(0)
#define	PIN_CS          4

//--------------------------------------------
// SPI Data Transfer Frequency (104MHz max)
// SPI1_CK = PCLK2(84MHz) / 2 = 42MHz
//#define SPI_CLK_DIV      0
// SPI1_CK = PCLK2(84MHz) / 32 = 2.625MHz
#define SPI_CLK_DIV      SPI_CR1_BR_2


//--------------------------------------------
static void hal_w25q_spi_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);
}

//--------------------------------------------
static void hal_w25q_spi_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
}

//--------------------------------------------
static uint8_t hal_w25q_spi_txrx(uint8_t data)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
}


void SPI_Flash_Init(void)
{	
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// IO port A clock enable
	// IO port B clock enable
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;// | RCC_AHB1ENR_GPIOBEN;

	hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MISO),   PIN_MISO,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_CS),     PIN_CS,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	// BIDIMODE = 0: 2-line unidirectional data mode selected
	// BIDIOE = 0: irrelevant
	// SSM = 1: Software slave management enabled
	// SSI = 1: NSS pin is ignored
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;

	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
	SPI_FLASH_TYPE=SPI_Flash_ReadID();//读取FLASH ID.  

}  
 
uint8_t SPI_Flash_ReadSR(void)   
{  
	uint8_t byte=0;   
	hal_w25q_spi_select();                            //使能器件   
	hal_w25q_spi_txrx(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=hal_w25q_spi_txrx(0Xff);             //读取一个字节  
	hal_w25q_spi_release();                            //取消片选     
	return byte;   
} 
 
void SPI_FLASH_Write_SR(uint8_t sr)   
{   
	hal_w25q_spi_select();                            //使能器件   
	hal_w25q_spi_txrx(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	hal_w25q_spi_txrx(sr);               //写入一个字节  
	hal_w25q_spi_release();                            //取消片选     	      
}   
   
void SPI_FLASH_Write_Enable(void)   
{
	hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_WriteEnable);      //发送写使能  
	hal_w25q_spi_release();                            //取消片选     	      
} 
  
void SPI_FLASH_Write_Disable(void)   
{  
	hal_w25q_spi_select();                            //使能器件   
    hal_w25q_spi_txrx(W25X_WriteDisable);     //发送写禁止指令    
	hal_w25q_spi_release();                            //取消片选     	      
} 			    
 
uint16_t SPI_Flash_ReadID(void)
{
	uint16_t Temp = 0;	  
	hal_w25q_spi_select();				    
	hal_w25q_spi_txrx(0x90);//发送读取ID命令	    
	hal_w25q_spi_txrx(0x00); 	    
	hal_w25q_spi_txrx(0x00); 	    
	hal_w25q_spi_txrx(0x00); 	 			   
	Temp|=hal_w25q_spi_txrx(0xFF)<<8;  
	Temp|=hal_w25q_spi_txrx(0xFF);	 
	hal_w25q_spi_release();				    
	return Temp;
}   		    
 
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;    												    
	hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ReadData);         //发送读取命令   
  hal_w25q_spi_txrx((uint8_t)((ReadAddr)>>16));  //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((ReadAddr)>>8));   
  hal_w25q_spi_txrx((uint8_t)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
  { 
      pBuffer[i]=hal_w25q_spi_txrx(0XFF);   //循环读数  
  }
	hal_w25q_spi_release();                            //取消片选     	      
}  

void SPI_Flash_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  SPI_FLASH_Write_Enable();                  //SET WEL 
	hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_PageProgram);      //发送写页命令   
  hal_w25q_spi_txrx((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((WriteAddr)>>8));   
  hal_w25q_spi_txrx((uint8_t)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)hal_w25q_spi_txrx(pBuffer[i]);//循环写数  
	hal_w25q_spi_release();                            //取消片选 
	SPI_Flash_Wait_Busy();					   //等待写入结束
} 

void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
 

uint8_t SPI_FLASH_BUF[4096];
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 	 
}
 

void SPI_Flash_Erase_Chip(void)   
{                                             
  SPI_FLASH_Write_Enable();                  //SET WEL 
  SPI_Flash_Wait_Busy();   
	hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ChipErase);        //发送片擦除命令  
	hal_w25q_spi_release();                            //取消片选     	      
	SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
}   



void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)   
{   
	Dst_Addr*=4096;
  SPI_FLASH_Write_Enable();                  //SET WEL 	 
  SPI_Flash_Wait_Busy();   
	hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_SectorErase);      //发送扇区擦除指令 
  hal_w25q_spi_txrx((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
  hal_w25q_spi_txrx((uint8_t)((Dst_Addr)>>8));   
  hal_w25q_spi_txrx((uint8_t)Dst_Addr);  
	hal_w25q_spi_release();                            //取消片选     	      
  SPI_Flash_Wait_Busy();   				   //等待擦除完成
}  


void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  

void SPI_Flash_PowerDown(void)   
{ 
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_PowerDown);        //发送掉电命令  
	hal_w25q_spi_release();                            //取消片选     	      
  //delay(3);                               //等待TPD  
}   
//唤醒
void SPI_Flash_WAKEUP(void)   
{  
  hal_w25q_spi_select();                            //使能器件   
  hal_w25q_spi_txrx(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	hal_w25q_spi_release();                            //取消片选     	      
  //delay(3);                               //等待TRES1
}   
