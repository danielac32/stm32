

#include "nvic.h"



//extern unsigned int _reset_handler;
static unsigned int __attribute__((aligned(256))) vector_table[16 + IRQ_COUNT];


void reset_handler(){}
void nmi_handler(){}
void hardfault_handler(){}

/*
void isr_svcall(){
	  __asm(
	    ".global svccall_handler_c\n"
	    "MRS r0, MSP\n"
	    "B svccall_handler_c\n"
	  );
}*/

/*void pendsv_handler(){
  printf("pendsv_handler\n");
}*/

extern void svccall_handler_c();
extern void pendsv_handler();
extern void systick_handler();
extern void isr_svcall();

void nvic_init() {
	int i;
	for (i = 0; i < (16 + IRQ_COUNT); ++i)
		vector_table[i] = 0;

	vector_table[1] = (unsigned int)reset_handler;
	vector_table[2] = (unsigned int)nmi_handler;
	vector_table[3] = (unsigned int)hardfault_handler;
	vector_table[11] = (unsigned int)isr_svcall;
	vector_table[14] = (unsigned int)pendsv_handler;
	vector_table[15] = (unsigned int)systick_handler;
	*((volatile unsigned int*)VTOR) = (unsigned int)vector_table;
}

void nvic_register_irq(unsigned int irq, void (*handler)()) {
	vector_table[16 + irq] = (unsigned int)handler;
}

void nvic_enable(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ISER)) = 1 << irq;
}

void nvic_disable(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ICER)) = 1 << irq;
}

void nvic_setpending(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ISPR)) = 1 << irq;
}

void nvic_clearpending(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ICPR)) = 1 << irq;
}

void irq_set_enabled(uint32_t irq, bool enabled) {
        uint32_t mask = (1<<irq);

        if (enabled) {
                write_reg(NVIC_ICPR, mask);
                write_reg(NVIC_ISER, mask);
        } else {
                write_reg(NVIC_ICER, mask);
        }
}