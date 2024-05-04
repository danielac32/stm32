
#include <w25q.h>

//--------------------------------------------
// GPIO_AF5_SPI1(APB2):
// SPI1 SCK:   PA5    PB3
// SPI1 MISO:  PA6    PB4
// SPI1 MOSI:  PA7    PB5
// GPIO_AF5_SPI2(APB1):
// SPI2 SCK:   PB10   PB13    PI0
// SPI2 MISO:  PC2    PB14    PI2
// SPI2 MOSI:  PC3    PB15    PI3
// GPIO_AF6_SPI3(APB1):
// SPI3 SCK:   PB3    PC10
// SPI3 MISO:  PB4    PC11
// SPI3 MOSI:  PB5    PC12

//--------------------------------------------
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
void hal_w25q_spi_init(void)
{
	// SPI1 clock enable
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
}

//--------------------------------------------
void hal_w25q_spi_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);
}

//--------------------------------------------
void hal_w25q_spi_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
}

//--------------------------------------------
uint8_t hal_w25q_spi_txrx(uint8_t data)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
}




/*
* Copyright (c) 2018 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

 #include <string.h>
#define ERASE_FN   4

typedef enum
{
	w25q80   = (uint16_t)0xEF13,
	w25q16   = (uint16_t)0xEF14,
	w25q32   = (uint16_t)0xEF15,
	w25q64   = (uint16_t)0xEF16,
	w25q128  = (uint16_t)0xEF17,
	no_flash = (uint16_t)0xFFFF
} flash_id;

typedef struct flash_info
{
	flash_id   id;                     // id of the flash
	uint32_t   flash_size;             // flash size in bytes
	uint32_t   page_size;              // page size in bytes
	uint32_t   erase_size[ERASE_FN];   // possible erase block sizes in bytes: sector, block1, block2, entire chip
} flash_info_t;

static flash_info_t flash_info;

//--------------------------------------------
static flash_id getid(void)
{
	uint16_t id;

	hal_w25q_spi_select();
	hal_w25q_spi_txrx(0x90);
	hal_w25q_spi_txrx(0x00);
	hal_w25q_spi_txrx(0x00);
	hal_w25q_spi_txrx(0x00);
	id = hal_w25q_spi_txrx(0xFF) << 8;
	id |= hal_w25q_spi_txrx(0xFF);
	hal_w25q_spi_release();
	return (flash_id)id;
}

//--------------------------------------------
static uint8_t readsr(uint8_t reg)
{
	uint8_t sr1;

	hal_w25q_spi_select();
	hal_w25q_spi_txrx(reg);
	sr1 = hal_w25q_spi_txrx(0xFF);
	hal_w25q_spi_release();
	return sr1;
}

//--------------------------------------------
static void writeenable(void)
{
	hal_w25q_spi_select();
	hal_w25q_spi_txrx(0x06);
	hal_w25q_spi_release();
}

//--------------------------------------------
static void waitforready(void)
{
	while (readsr(0x05) & 0x01);
}

//--------------------------------------------
void w25q_init(void)
{
	memset(&flash_info, sizeof(flash_info), 0);
	hal_w25q_spi_init();
	flash_info.id = getid();
	switch (flash_info.id)
	{
	case w25q80:
		flash_info.flash_size = 1024UL * 1024UL * 1UL;
		break;
	case w25q16:
		flash_info.flash_size = 1024UL * 1024UL * 2UL;
		break;
	case w25q32:
		flash_info.flash_size = 1024UL * 1024UL * 4UL;
		break;
	case w25q64:
		flash_info.flash_size = 1024UL * 1024UL * 8UL;
		break;
	case w25q128:
		flash_info.flash_size = 1024UL * 1024UL * 16UL;
		break;
	default:
		flash_info.flash_size = 1024UL * 1024UL * 8UL;
		break;
	}
	flash_info.page_size = 256UL;
	flash_info.erase_size[0] = 256UL * 16UL;          // sector erase (4KB)
	flash_info.erase_size[1] = 256UL * 128UL;         // block erase  (32KB)
	flash_info.erase_size[2] = 256UL * 256UL;         // block erase  (64KB)
	flash_info.erase_size[3] = flash_info.flash_size; // chip erase
}

//--------------------------------------------
void w25q_read(uint8_t *buf, uint32_t addr, uint32_t size)
{
	uint32_t cnt;

	hal_w25q_spi_select();
	hal_w25q_spi_txrx(0x03);
	hal_w25q_spi_txrx((uint8_t)(addr >> 16));
	hal_w25q_spi_txrx((uint8_t)(addr >> 8));
	hal_w25q_spi_txrx((uint8_t)(addr));
	for (cnt = 0; cnt < size; cnt++)
	{
		buf[cnt] = hal_w25q_spi_txrx(0xFF);
	}
	hal_w25q_spi_release();
}

//--------------------------------------------
void w25q_writepage(uint8_t *buf, uint32_t addr, uint32_t size)
{
	uint32_t cnt;
	uint32_t pagebeg;
	uint32_t pageend;

	pagebeg = addr / flash_info.page_size;
	pageend = (addr + size - 1) / flash_info.page_size;
	if ((size > flash_info.page_size) ||
		(pagebeg != pageend))
	{
		return;
	}
	writeenable();
	waitforready();
	hal_w25q_spi_select();
	hal_w25q_spi_txrx(0x02);
	hal_w25q_spi_txrx((uint8_t)(addr >> 16));
	hal_w25q_spi_txrx((uint8_t)(addr >> 8));
	hal_w25q_spi_txrx((uint8_t)(addr));
	for (cnt = 0; cnt < size; cnt++)
	{
		hal_w25q_spi_txrx(buf[cnt]);
	}
	hal_w25q_spi_release();
	waitforready();
}

//--------------------------------------------
static void chiperase(void)
{
	writeenable();
	waitforready();
	hal_w25q_spi_select();
	hal_w25q_spi_txrx(0x60);
	hal_w25q_spi_release();
	waitforready();
}

//--------------------------------------------
static void blockerase(uint8_t cnt, uint32_t addr)
{
	uint8_t cmd;

	switch (cnt)
	{
	case 0:
		cmd = 0x20;    // sector erase (4KB)
		break;
	case 1:
		cmd = 0x52;    // block erase  (32KB)
		break;
	case 2:
		cmd = 0xD8;    // block erase  (64KB)
		break;
	default:
		return;
	}

	writeenable();
	waitforready();
	hal_w25q_spi_select();
	hal_w25q_spi_txrx(cmd);
	hal_w25q_spi_txrx((uint8_t)(addr >> 16));
	hal_w25q_spi_txrx((uint8_t)(addr >> 8));
	hal_w25q_spi_txrx((uint8_t)(addr));
	hal_w25q_spi_release();
	waitforready();
}

//--------------------------------------------
static void erase(uint32_t *addr, uint32_t *size)
{
	uint32_t len;
	int8_t cnt;

	for (cnt = ERASE_FN - 2; cnt >= 0; cnt--)
	{
		len = flash_info.erase_size[cnt];
		if ((*addr % len == 0) && (((int32_t)(*size) - (int32_t)len) >= 0))
		{
			blockerase(cnt, *addr);
			*size -= len;
			*addr += len;
			return;
		}
	}
}

//--------------------------------------------
// addr: must be aligned to the sector start address
// size: must be aligned to the sector size
void w25q_erase(uint32_t addr, uint32_t size)
{
	if (addr + size > flash_info.flash_size)
	{
		return;
	}

	if (addr % flash_info.erase_size[0] || size % flash_info.erase_size[0])
	{
		return;
	}

	if (addr == 0 && size == flash_info.flash_size)
	{
		chiperase();
		return;
	}

	while (size)
	{
		erase(&addr, &size);
	}
}

//--------------------------------------------
uint32_t w25q_getflashsize(void)
{
	return flash_info.flash_size;
}

//--------------------------------------------
uint32_t w25q_getpagesize(void)
{
	return flash_info.page_size;
}

//--------------------------------------------
uint32_t w25q_geterasesectorsize(void)
{
	return flash_info.erase_size[0];
}


const spi_flash_drv_t flash_drv =
{
	w25q_init,
	w25q_read,
	w25q_writepage,
	w25q_erase,
	w25q_getflashsize,
	w25q_getpagesize,
	w25q_geterasesectorsize
};