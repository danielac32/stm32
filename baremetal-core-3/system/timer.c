#include <xinu.h>


static uint32 count_timer[5];
// timers
volatile TIM_TypeDef* TIMERS[] = {
	(TIM_TypeDef*)TIM1_BASE,
	(TIM_TypeDef*)TIM2_BASE,
	(TIM_TypeDef*)TIM3_BASE,
	(TIM_TypeDef*)TIM4_BASE,
	(TIM_TypeDef*)TIM5_BASE
};

// ISR jump table
void (*TIMER_HANDLERS[4])(volatile TIM_TypeDef*);


void timer_init(uint8 timer, uint16 prescaler, uint16 period) {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[timer];
	// all channels off
	tim->CCER = 0;
	// set prescaler
	tim->PSC = prescaler - 1;
	// set period: update when counter reaches this number
	tim->ARR = period - 1;
	// enable timer
	tim->CR1 = 0x01;
	count_timer[timer]=0;
}


void timer_init_int(uint8 timer, void (*handler)) {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[timer];
	// enable overflow interrupt
	tim->DIER |= 0b1;
	// register in nvic
	switch (timer) {
		case TIMER1:
			enable_interrupt(TIM1_UP_TIM10_IRQn);
			break;
		case TIMER2:
			enable_interrupt(TIM2_IRQn);
			break;
		case TIMER3:
			enable_interrupt(TIM3_IRQn);
			break;
		case TIMER4:
			enable_interrupt(TIM4_IRQn);
			break;
		default:
			break;
	}
	// save iSR pointer in jump table
	TIMER_HANDLERS[timer] = handler;
}


uint16 timer_get_count(uint8 timer) {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[timer];
	// return current timer value
	return tim->CNT;
}

uint32 get_timer(uint8 timer){
return count_timer[timer];
}


void TIM1_UP_TIM10_IRQHandler() {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[TIMER1];
	// check if update interrupt
	if(tim->SR & 0b1) {
		// check if iSR is in jump table
		if(TIMER_HANDLERS[TIMER1]) {
			(*TIMER_HANDLERS[TIMER1])(tim);
		}
	}
	count_timer[TIMER1]++;
	// clear status
	tim->SR = 0;
}

// Timer 2 ISR
void TIM2_IRQHandler() {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[TIMER2];
	// check if update interrupt
	if(tim->SR & 0b1) {
		// check if iSR is in jump table
		if(TIMER_HANDLERS[TIMER2]) {
			(*TIMER_HANDLERS[TIMER2])(tim);
		}
	}
	count_timer[TIMER2]++;
	// clear status
	tim->SR = 0;
}

// Timer 3 ISR
void TIM3_IRQHandler() {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[TIMER3];
	// check if update interrupt
	if(tim->SR & 0b1) {
		// check if iSR is in jump table
		if(TIMER_HANDLERS[TIMER3]) {
			(*TIMER_HANDLERS[TIMER3])(tim);
		}
	}
	count_timer[TIMER3]++;
	// clear status
	tim->SR = 0;
}

// Timer 4 ISR
void TIM4_IRQHandler() {
	// timer pointer
	volatile TIM_TypeDef* tim = TIMERS[TIMER4];
	// check if update interrupt
	if(tim->SR & 0b1) {
		// check if iSR is in jump table
		if(TIMER_HANDLERS[TIMER4]) {
			(*TIMER_HANDLERS[TIMER4])(tim);
		}
	}
	count_timer[TIMER4]++;
	// clear status
	tim->SR = 0;
}