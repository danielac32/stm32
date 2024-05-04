 
#include <stdint.h>



#define BLOCK 0xfff
extern void cachebegin(uint32_t size);
extern void cachewrite(uint32_t address,uint8 value);
extern uint8_t cacheread(uint32_t address);
extern uint32_t cacheget_size();

extern uint8_t lowmemRAM[];
extern uint8_t cacheRAM1[];
extern uint8_t cacheRAM2[];
//extern uint8_t cacheRAM3[];
//extern uint8_t cacheRAM4[];

extern uint32_t block1;
extern uint32_t block2;
//extern uint32_t block3;
//extern uint32_t block4;

extern uint8_t dirty1;
extern uint8_t dirty2;
//extern uint8_t dirty3;
//extern uint8_t dirty4;

extern uint8_t NextCacheUse;
extern uint32_t max;
