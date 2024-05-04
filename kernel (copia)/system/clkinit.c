/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* clkinit.c - clkinit STM32 */

#include <xinu.h>

uint32	clktime;		/* Seconds since boot			*/
uint32  count1000;              /* ms since last clock tick             */
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void clkinit(void)
{
	
	 

	sleepq = newqueue();	/* Allocate a queue to hold the delta	*/
				/*   list of sleeping processes		*/
	preempt = QUANTUM;	/* Set the preemption time		*/
	clktime = 0;		/* Start counting seconds		*/
    count1000 = 0;
 
	//RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    
    //Set the prescalar and the ARR register
    //TIM9->PSC = 100;
    //TIM9->ARR = 0xFFFF;

    // Enable timer and wait for update flag to set
    //TIM9->CR1 |= (1 << 0);
    //while (!(TIM9->SR & (1<<0)));
    
    
    

    //NVIC->IPR[TIM3_IRQn] = 0x10;
    
    //TIM9->DIER |= (1 << 0);
    //enable_interrupt(TIM1_BRK_TIM9_IRQn);


   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
 
   TIM3->CR1 = 0x0000;

  // fCK_PSC / (PSC[15:0] + 1)
  // 72 Mhz / 71 + 1 = 1 Mhz timer clock speed
   TIM3->PSC = 71;

  // This is set to max value (0xFFFF) since we manually check
  //   if the value reach to 1000 in the delay_ms function
   TIM3->ARR = 0xFFFF;

  // Finally enable TIM3 module
   TIM3->CR1 |= (1 << 0);
 

   init_systick(50,1);

	return;
}
