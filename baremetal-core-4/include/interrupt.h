#include "stdint.h"
//#include "stm32f411.h"
/* interrupt.h */

extern uint64_t clockticks; // rolls over after 2^64/96MHz = 6089.1097 years
extern uint64_t   cycleCount(void);
extern void delay(uint32_t usec);
extern void irq_enable(void);
extern uint32_t irq_disable(void);
extern uint32_t irq_restore(uint32_t irq);


