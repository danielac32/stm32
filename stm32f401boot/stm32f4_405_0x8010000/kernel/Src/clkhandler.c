#include <kernel.h>
#include <interrupt.h>
#include <process.h>
#include <queue.h>

/*-----------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *-----------------------------------------------------------------------
 */

//void __attribute__ ((naked)) clkhandler()



void TIM2_Handler()
{ 
  TIM2->SR &= ~(1U << 0);
  sys_time += 1;
  if(ready_preemptive){
        
        task_t *tmp = queue_peek(&sleep_queue);
        while (tmp) {
            if (sys_time >= tmp->param) { // si se cumplio el tiempo sleep
                tmp = queue_pop(&sleep_queue);//sacalo del queue sleep
                
                ready(tmp->pid);
               // tmp->state = PR_CURR;// cambia el status
               // queue_push(&ready_queue, tmp);
                tmp = queue_peek(&sleep_queue);
            } else {
                break;
            }

        }
        if((--preempt) == 0) {
            preempt = QUANTUM;

            PEND_SV(); 
        } 
  } 
}


