#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "riscv.h"


#define MTIM 7
#define MSIT 3
#define MEIT 11

extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int num);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int num);
extern void clear_interrupts();
extern void soft_trap();
#define timel *(uint32_t*)0x11004000
#define timeh *(uint32_t*)0x11004004

extern void trap_vector();

// These are going to be bound to memory addresses in the linker script.
extern uint32_t SYSCON;
extern uint32_t TIMERL;

// This is just a definition for a symbol found in the .S file.

// These will not turn into function calls, but instead will find a way
// of writing the assembly in-line
static void lprint( const char * s )
{
	asm volatile( ".option norvc\ncsrrw x0, 0x138, %0\n" : : "r" (s));
}

static void pprint( intptr_t ptr )
{
	asm volatile( ".option norvc\ncsrrw x0, 0x137, %0\n" : : "r" (ptr));
}

static void nprint( int ptr )
{
	asm volatile( ".option norvc\ncsrrw x0, 0x136, %0\n" : : "r" (ptr));
}

static inline uint32_t get_cyc_count() {
	uint32_t ccount;
	asm volatile(".option norvc\ncsrr %0, 0xC00":"=r" (ccount));
	return ccount;
}


int lib_putc(char ch)
{
    while ((*UART_LSR & UART_LSR_EMPTY_MASK) == 0)
        ;
    return *UART_THR = ch;
}

void lib_puts(char *s)
{
    while (*s)
        lib_putc(*s++);
}



 

/*
uint32_t exceptionHandler(int epc,int cause){

  uint32_t return_pc = epc;
  uint32_t cause_code = cause & 0xfff;
  
  if (cause & 0x80000000){

  		switch (cause_code)
	    {
	    case 3:
	      lprint("software interruption!\n");
	      break;
	    case 7:
	      lprint("timer interruption!\n");
	      // disable machine-mode timer interrupts.
	      w_mie(~((~r_mie()) | (1 << 7)));
	      //timer_handler();
	      //return_pc = (reg_t)&os_kernel;
	      // enable machine-mode timer interrupts.
	      w_mie(r_mie() | MIE_MTIE);
	      break;
	    case 11:
	      lprint("external interruption!\n");
	      break;
	    default:
	      lprint("unknown async exception!\n");
	      break;
	    }

  }else{
   
    lprint("Sync exceptions!\n");
   
  }
  return return_pc;


//pprint(a);
//pprint(b);
// clear_interrupts();
}
 */


void softTrapHandler(int a) {
    lprint("soft trap\n");
    clear_interrupts();
}

extern void _trap();

#define csr_write(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })




/*
	PROVIDE( TIMERL = 0x1100bff8 );
	PROVIDE( TIMERH = 0x1100bffc );
	*/


#define csr_set(reg, bit) ({ unsigned long __tmp; \
  asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
  __tmp; })


int main()
{


    

lprint("test uart\n");
enable_interrupt_status();
enable_interrupt(MSIT);
//timel = 0xfffff;
//timeh = 0;

//w_mtvec((reg_t)_trap);

/*clear_interrupts();
enable_interrupt_status();
enable_interrupt(MTIM);
w_mtvec((reg_t)_trap);
w_mstatus(r_mstatus() | MSTATUS_MIE);
w_mie(r_mie() | MIE_MTIE);
csr_write(0xc01,10000);
// w_mie(r_mie() | MIE_MSIE);
//soft_trap();*/

//soft_trap();
 
 SYSCON = 0x5555;
while(1);
	/*lprint("\n");
	lprint("Hello world from RV32 land.\n");
	lprint("main is at: ");
	pprint( (intptr_t)main );
	lprint("\nAssembly code: ");
	asm_demo_func();
	lprint("\n");

	// Wait a while.
	uint32_t cyclecount_initial = get_cyc_count();
	uint32_t timer_initial = TIMERL;

	volatile int i;
	for( i = 0; i < 1000000; i++ )
	{
		asm volatile( "nop" );
	}

	// Gather the wall-clock time and # of cycles
	uint32_t cyclecount = get_cyc_count() - cyclecount_initial;
	uint32_t timer = TIMERL - timer_initial;

	lprint( "Processor effective speed: ");
	nprint( cyclecount / timer );
	lprint( " Mcyc/s\n");

	lprint("\n");*/
	SYSCON = 0x5555; // Power off
}

