#include <stdint.h>
#include <interrupt.h>
#include <string.h>
#include <stm32.h>



uint64_t clockticks = STK_LOAD_RELOAD + 1; // rolls over after 2^64/96MHz = 6089.1097 years

uint64_t   cycleCount(void) { 
	    return clockticks - (uint32_t)(SysTick->VAL & STK_LOAD_RELOAD); 
}

void   SysTick_Handler(void) { 
      clockticks += STK_LOAD_RELOAD + 1; 
}

void delay(uint32_t usec) {
     uint64_t now = cycleCount();
     // then = now + usec * clockspeed_hz / (usec/sec)
     uint64_t then = now + 100 * usec;
     while (cycleCount() < then)
         __NOP(); // wait
 }

