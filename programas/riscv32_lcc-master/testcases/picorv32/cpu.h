#include <stdint.h>

#ifndef CPU_H_
#define CPU_H_


typedef struct rv32_core_struct
{
  uint32_t opcode;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t funct3;
  uint32_t insn;
  int32_t imm;
  uint32_t cond;
  uint32_t err;
  uint32_t addr;
  uint32_t val;
  uint32_t val2;
  uint32_t temp;
  char *memory;
  uint8_t running;
} rv32_core_td;


struct rv32{
uint8_t next;
uint8_t debug_inst;
char *memory;//[0x000595b0];
uint8_t running;
uint8_t pending_exception;
uint32_t pending_tval;
uint32_t pc;
uint32_t insn;
uint32_t reg[32];
};
struct rv32 riscv;

void openconf(char *);
void openrom(char *);
 
void raise_exception(uint32_t cause, uint32_t tval);
uint8_t target_read_u8(uint8_t *pval, uint32_t addr);
uint8_t target_read_u16(uint16_t *pval, uint32_t addr);
uint8_t target_read_u32(uint32_t *pval, uint32_t addr);
uint8_t target_write_u8(uint32_t addr, uint8_t val);
uint8_t target_write_u16(uint32_t addr, uint16_t val);
uint8_t target_write_u32(uint32_t addr, uint32_t val);
int32_t div32(int32_t a, int32_t b);
uint32_t divu32(uint32_t a, uint32_t b);
int32_t rem32(int32_t a, int32_t b);
uint32_t remu32(uint32_t a, uint32_t b);
uint32_t mulh32(int32_t a, int32_t b);
uint32_t mulhsu32(int32_t a, uint32_t b);
uint32_t mulhu32(uint32_t a, uint32_t b);
void dump_regs();
void execute_instruction();
void init_soc();
 

#define XLEN 32

#define base_io 0xFA000
#define ioaddr(x) base_io + x
#define UART_TX_ADDR    ioaddr(2) 


 
// exception causes
#define CAUSE_MISALIGNED_FETCH    0x0
#define CAUSE_FAULT_FETCH         0x1
#define CAUSE_ILLEGAL_INSTRUCTION 0x2
#define CAUSE_BREAKPOINT          0x3
#define CAUSE_MISALIGNED_LOAD     0x4
#define CAUSE_FAULT_LOAD          0x5
#define CAUSE_MISALIGNED_STORE    0x6
#define CAUSE_FAULT_STORE         0x7
#define CAUSE_USER_ECALL          0x8
#define CAUSE_SUPERVISOR_ECALL    0x9
#define CAUSE_HYPERVISOR_ECALL    0xa
#define CAUSE_MACHINE_ECALL       0xb
#define CAUSE_FETCH_PAGE_FAULT    0xc
#define CAUSE_LOAD_PAGE_FAULT     0xd
#define CAUSE_STORE_PAGE_FAULT    0xf
#endif

