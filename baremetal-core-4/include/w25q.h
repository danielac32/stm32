
#include <stdint.h>
#include <stm32.h>
#include <gpio.h>

typedef struct spi_flash_drv
{
	void (*init) (void);
	void (*read) (uint8_t *buf, uint32_t addr, uint32_t size);
	void (*write_page) (uint8_t *buf, uint32_t addr, uint32_t size);
	void (*erase) (uint32_t addr, uint32_t size);
	uint32_t (*get_flash_size) (void);
	uint32_t (*get_page_size) (void);
	uint32_t (*get_erase_sector_size) (void);
} spi_flash_drv_t;

void hal_w25q_spi_init(void);
void hal_w25q_spi_select(void);
void hal_w25q_spi_release(void);
uint8_t hal_w25q_spi_txrx(uint8_t data);

void w25q_init(void);
void w25q_read(uint8_t *buf, uint32_t addr, uint32_t size);
void w25q_writepage(uint8_t *buf, uint32_t addr, uint32_t size);
void w25q_erase(uint32_t addr, uint32_t size);
uint32_t w25q_getflashsize(void);
uint32_t w25q_getpagesize(void);
uint32_t w25q_geterasesectorsize(void);