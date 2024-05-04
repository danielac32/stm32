#ifndef SRAM_H_
#define SRAM_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2

#define CS_RAM1  2
#define CS_RAM2  0

class SRAMClass {
public:
      SRAMClass();
      void write(uint32_t address, uint8_t data_byte);
      uint8_t read(uint32_t address);
      uint8_t readBlock(uint32_t address,uint8_t *data,int size);
      void writeBlock(uint32_t address, uint8_t *data,int size);
protected:
      
};
#endif
