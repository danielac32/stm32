
#define KEY1 0x45670123
#define KEY2 0xCDEF89AB
extern void unlock_flash();
extern void lock_flash();
extern void writeflash(uint32 addr, uint32 data);
extern uint32 readflash(uint32 Address);
extern void erase_flash_sector(uint32 sector);