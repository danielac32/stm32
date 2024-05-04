#include "stdint.h"
#include "stm32.h"
#include "kernel.h"
/* interrupt.h */


extern void clkinit();
extern uint64_t clockticks; // rolls over after 2^64/96MHz = 6089.1097 years
extern uint64_t   cycleCount(void);
extern void delay(uint32_t usec);
//extern void delay_ms(uint32_t delay_ms);
extern void delay_us(uint32_t delay_us);
extern void irq_enable(void);
extern uint32_t irq_disable(void);
extern uint32_t irq_restore(uint32_t irq);
extern void halt();

extern	uint32	clktime;	/* current time in secs since boot	*/
extern  uint32  count1000;      /* ms since last clock tick             */

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	int32	slnonempty;	/* nonzero if sleepq is nonempty	*/
extern	int32	*sltop;		/* ptr to key in first item on sleepq	*/
extern	uint32	preempt;	/* preemption counter			*/
