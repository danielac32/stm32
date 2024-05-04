 

 #include "kernel.h"

// Timers
#define TIMER1	0
#define TIMER2	1
#define TIMER3	2
#define TIMER4	3
#define TIMER5	4



void timer_init(uint8 timer, uint16 prescaler, uint16 period);
void timer_init_int(uint8 timer, void (*handler));
uint16 timer_get_count(uint8 timer);
uint32 get_timer(uint8 );