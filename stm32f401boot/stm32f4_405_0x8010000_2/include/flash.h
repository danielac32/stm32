// flash unlock sequence
#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

// variable that will be hold in the flash
#define VARADDR 0x0800C000 // address that will hold the variable
#define VAR     0x12345670 // variable value
#define FLASH_BASE2 ((uint32_t)0x08010000) 

void unlock_flash();
void lock_flash();
void erase_flash_sector(uint32_t sec);
void erase_flash_sector3();
void erase_flash_sector4();
void erase_flash_sector5();
void erase_flash_sector6();
void erase_flash_sector7();
void erase_flash_sector8();
void erase_flash_sector9();
void erase_flash_sector10();
void erase_flash_sector11();
void erase_flash_sector12();
void write_flash(uint32_t addr, uint32_t data);
char *getUrlTargetFileBoot();
void setUrlTargetFileBoot(char *str);



