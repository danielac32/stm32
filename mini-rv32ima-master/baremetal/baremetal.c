#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


#define MTIM 7
#define MSIT 3
#define MEIT 11

extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int num);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int num);
extern void clear_interrupts();
extern void soft_trap();


// These are going to be bound to memory addresses in the linker script.
extern uint32_t SYSCON;
extern uint32_t TIMERL;

// This is just a definition for a symbol found in the .S file.
void asm_demo_func();

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

static void nprint( intptr_t ptr )
{
	asm volatile( ".option norvc\ncsrrw x0, 0x136, %0\n" : : "r" (ptr));
}

static inline uint32_t get_cyc_count() {
	uint32_t ccount;
	asm volatile(".option norvc\ncsrr %0, 0xC00":"=r" (ccount));
	return ccount;
}


void softTrapHandler(int a) {
    lprint("soft trap\n");
    clear_interrupts();
}


int main()
{
    clear_interrupts();
	enable_interrupt_status();
    enable_interrupt(MSIT);
    enable_interrupt(MSIT);
    enable_interrupt(MEIT);

	soft_trap();

    while(1);
	SYSCON = 0x5555; // Power off
}

