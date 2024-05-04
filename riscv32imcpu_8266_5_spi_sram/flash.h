#ifndef FLASH_H_
#define FLASH_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define BLOCK 4096


class FLASHClass {
public:
     FLASHClass();
     void begin(uint32_t size);
     void write(uint32_t address,uint8_t value);
     uint8_t read(uint32_t address);
     uint32_t get_size();
protected:
  uint8_t lowmemRAM[1024];
  uint8_t cacheRAM1[BLOCK];
  uint8_t cacheRAM2[BLOCK];
  uint8_t cacheRAM3[BLOCK];
  uint8_t cacheRAM4[BLOCK];

  uint32_t block1=0;
  uint32_t block2=0;
  uint32_t block3=0;
  uint32_t block4=0;
 
  uint8_t dirty1=0;
  uint8_t dirty2=0;
  uint8_t dirty3=0;
  uint8_t dirty4=0;
 
  uint8_t NextCacheUse=0;
  uint32_t max=0;
};
 

#endif
