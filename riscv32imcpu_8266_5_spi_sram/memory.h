#include <stdint.h>

#ifndef MEMORY_H_
#define MEMORY_H_


uint8_t memory_read(uint32_t addr);
void memory_write(uint32_t addr,uint8_t val);
uint8_t read_vm(uint32_t addr);
void write_vm(uint32_t addr,uint8_t val);
void w8m(uint32_t addr, uint8_t val);
uint8_t r8m(uint32_t addr);
void w16m(uint32_t addr, uint16_t val);
uint16_t r16m(uint32_t addr);
void w32m(uint32_t addr, uint32_t val);
uint32_t r32m(uint32_t addr);


uint8_t target_read_u8(uint8_t *pval, uint32_t addr);
uint8_t target_read_u16(uint16_t *pval, uint32_t addr);
uint8_t target_read_u32(uint32_t *pval, uint32_t addr);
uint8_t target_write_u8(uint32_t addr, uint8_t val);
uint8_t target_write_u16(uint32_t addr, uint16_t val);
uint8_t target_write_u32(uint32_t addr, uint32_t val);
uint32_t get_op(uint32_t addr);
void WriteMemory(uint32_t addr,uint8_t *data,uint8_t size);
void ReadMemory(uint32_t addr,uint8_t *data,uint8_t size);

int kbhit();



#endif
