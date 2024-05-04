#include <xinu.h>
#include <flash.h>

/*

 * [Bank 1]
 * Sector 0  | 0x0800_0000 - 0x0800_3FFF | 16Kbytes
 * Sector 1  | 0x0800_4000 - 0x0800_7FFF | 16Kbytes
 * Sector 2  | 0x0800_8000 - 0x0800_BFFF | 16Kbytes
 * Sector 3  | 0x0800_C000 - 0x0800_FFFF | 16Kbytes
 * Sector 4  | 0x0801_0000 - 0x0801_FFFF | 64Kbytes
 * Sector 5  | 0x0802_0000 - 0x0803_FFFF | 128Kbytes
 * Sector 6  | 0x0804_0000 - 0x0805_FFFF | 128Kbytes
 * Sector 7  | 0x0806_0000 - 0x0807_FFFF | 128Kbytes
 * Sector 8  | 0x0808_0000 - 0x0809_FFFF | 128Kbytes
 * Sector 9  | 0x080A_0000 - 0x080B_FFFF | 128Kbytes
 * Sector 10 | 0x080C_0000 - 0x080D_FFFF | 128Kbytes
 * Sector 11 | 0x080E_0000 - 0x080F_FFFF | 128Kbytes
 * [Bank 2]
 * Sector 12 | 0x0810_0000 - 0x0810_3FFF | 16Kbytes
 * Sector 13 | 0x0810_4000 - 0x0810_7FFF | 16Kbytes
 * Sector 14 | 0x0810_8000 - 0x0810_BFFF | 16Kbytes
 * Sector 15 | 0x0810_C000 - 0x0810_FFFF | 16Kbytes
 * Sector 16 | 0x0811_0000 - 0x0811_FFFF | 64Kbytes
 * Sector 17 | 0x0812_0000 - 0x0813_FFFF | 128Kbytes
 * Sector 18 | 0x0814_0000 - 0x0815_FFFF | 128Kbytes
 * Sector 19 | 0x0816_0000 - 0x0817_FFFF | 128Kbytes
 * Sector 20 | 0x0818_0000 - 0x0819_FFFF | 128Kbytes
 * Sector 21 | 0x081A_0000 - 0x081B_FFFF | 128Kbytes
 * Sector 22 | 0x081C_0000 - 0x081D_FFFF | 128Kbytes
 * Sector 23 | 0x081E_0000 - 0x081F_FFFF | 128Kbytes



 */
#if 0
#define ADDR_FLASH_SECTOR_0     ((uint32)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#endif

void unlock_flash() {
    if (FLASH->CR & FLASH_CR_LOCK) {
        FLASH->KEYR = KEY1;
        FLASH->KEYR = KEY2;
    }
}

void lock_flash() {
    FLASH->CR |= FLASH_CR_LOCK; // bit 31
}


void erase_flash_sector(uint32 sector) {
    uint32 q=disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sector << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    restore(q);
}
 
void writeflash(uint32 addr, uint32 data){
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_PG;
    FLASH->CR &= ~(0x3U << 8); // clear PSIZE bit 8:9
    FLASH->CR |= (0x2 << 8);   // program PSIZE
    *(volatile uint32*)addr = data;
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= (~FLASH_CR_PG); // disable PG bit
}

// Read 32-bit value from EEPROM data memory
// input:
//   Address - address in data memory
// return: value from EEPROM data memory
// note: Address must be between DATA_EEPROM_START_ADDR and DATA_EEPROM_END_ADDR
uint32 readflash(uint32 Address) {
	return (*(volatile uint32*)Address);
}

// Read specified quantity of data from EEPROM to buffer
// input:
//   addr - start address in EEPROM data memory
//   buffer - pointer to buffer
//   len - length of buffer in bytes
// note: buffer must be 32-bit aligned

/*
void ReadBuffer_EEPROM(uint32 addr, volatile uint32 *buffer, uint32 len) {
	uint8_t i;

	for (i = 0; i < len; i += 4) *buffer++ = EEPROM_Read(addr + i);
}

// Save specified buffer to EEPROM
// input:
//   addr - start address in EEPROM data memory
//   buffer - pointer to buffer
//   len - length of buffer in bytes
// note: buffer must be 32-bit aligned
void SaveBuffer_EEPROM(uint32 addr, volatile uint32 *buffer, uint32 len) {
	uint8_t i;
	volatile uint32 data;

	EEPROM_Unlock();
	for (i = 0; i < len; i += 4) {
		data = *buffer++;
		if (data != EEPROM_Read(addr + i)) EEPROM_Write(addr + i,data);
	}
	EEPROM_Lock();
}*/