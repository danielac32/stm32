#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <usbapi.h>
#include <queue.h>
#include <memory.h>
#include <kernel.h>
#include <littlefs.h>
#include <process.h>

queue_t ready_queue;
queue_t sleep_queue;
task_t *active_task=NULL;
uint32 sys_time;
uint32 preempt;
int ready_preemptive=0;
task_t task[MAX_NUMBER_OF_TASKS];
//task_t thrtab[NTHREAD];
//int thrcount;                   /* Number of live user threads         */
//int thrcurrent; 

void svc_handler(uint32_t *registers) {
kprintf("aquii");
kprintf("syscall %d\n",registers[0]);

}


local int   newpid(void)
{
    uint32  i;          /* iterate through all processes*/
//  static  pid32 nextpid = 1;  /* position in table to try or  */
     int nextpid = 0;  /* position in table to try or  */

    for (i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
        if(task[i].state == PR_FREE) {
            return nextpid++;
        } else {
            nextpid++;
        }
    }
    return (int) -1;
}
 
/*
void TIM2_IRQHandler(){
	if (TIM2->DIER & 0x01) {
        if (TIM2->SR & 0x01) {
            TIM2->SR &= ~(1U << 0);
        }
    }
    
    if(ready_preemptive){
    	 
	    if((--preempt) == 0) {
	        preempt = QUANTUM;
	        PEND_SV(); 
	    }
    }
}*/

void sleep(uint32 ticks) {
  uint32 mask=irq_disable();
  if (ticks) {
      active_task->state = PR_SLEEP;
      active_task->param = ticks + clockticks;//*1000;
      queue_pushsort(&sleep_queue, active_task);
     // queue_push(&sleep_queue, active_task);
  }
  irq_restore(mask);
  //syscall(XINU_YIELD);//
  PEND_SV();
}



void kill(int pid){

  uint32 mask=irq_disable();
  //char *state[]={"free ", "curr ", "ready", "recv ", "sleep", "susp ","wait "};

  task_t *prptr;
 prptr =& task[pid];
  
  free(prptr->prstkbase);

  if(prptr->elf == TRUE){
     //kprintf("killed %s\n", prptr->name);
     free(prptr->img);//free(prptr->img);
     prptr->elf = FALSE;
  }


  switch(prptr->state){
        case PR_CURR:
             prptr->state=PR_FREE;
        break;
        case PR_SLEEP:
             {
	              task_t *tmp = queue_peek(&sleep_queue);
	              while (tmp->state==PR_SLEEP) {
	                    tmp = queue_pop(&sleep_queue);

	                    if (tmp->pid==prptr->pid) {
	                        //tmp = queue_pop(&sleep_queue);
	                        tmp->state = PR_FREE;
	                        break;
	                    }else{
	                        queue_push(&sleep_queue, tmp);
	                    }
	              }
             }
        break;
        case PR_READY:
              {
	              task_t *tmp = queue_peek(&ready_queue);
	              while (tmp->state==PR_READY) {
	                    tmp = queue_pop(&ready_queue);

	                    if (tmp->pid==prptr->pid) {
	                        //tmp = queue_pop(&sleep_queue);
	                        tmp->state = PR_FREE;
	                        //kprintf("");//
	                        //kprintf("ready .killed  %d--%s\n", tmp->pid,tmp->name);
	                        break;
	                    }else{
	                        queue_push(&ready_queue, tmp);
	                    }
	              }
              
             }
        break;
        default:
            
            kprintf("default .killed  %d--%s \n", prptr->pid,prptr->name);
            prptr->state = PR_FREE;
            break;
            
  }
  irq_restore(mask);
}


local void killer(){
      int pid = getpid();
      kill(pid);
      PEND_SV();
      while(1);
}

int getpid(){
  return active_task->pid;
}



static void *cmcm_push_context(void) {
  void *psp;

  // copies registers to the PSP stack
  // additional registers are pushed in hardware
  __asm__("MRS %0, psp\n"
          "STMDB %0!, {r4-r11}\n"
          "MSR psp, %0\n"
          : "=r" (psp));

  return psp;
}

static void cmcm_pop_context(void *psp) {
  // loads registers with contents of the PSP stack
  // additional registers are popped in hardware
  __asm__("LDMFD %0!, {r4-r11}\n"
          "MSR psp, %0\n"
          : : "r" (psp));
}
/*
void PendSV_Handler(){

   if (active_task != NULL){
       active_task->sp = cmcm_push_context();
       active_task->state=PR_READY;
       queue_push(&ready_queue, active_task);
   }

   //if (active_task->state == PR_CURR  && active_task->state != PR_FREE) {
     // queue_push(&ready_queue, active_task);
   //}

   active_task = queue_pop(&ready_queue);
   active_task->state=PR_CURR;
   preempt = QUANTUM;
   cmcm_pop_context(active_task->sp);
  __asm__("bx %0" : : "r" (0xFFFFFFFD));
}
*/
int ready(int pid/*, bool resch*/)
{
    task_t *prptr;
    prptr =& task[pid];
    prptr->state = PR_CURR;
    queue_push(&ready_queue, prptr); 
    return OK;
}













int ilde2(){
    int a;
    while(1){
         printf("ilde\n" );
    }
    return 0;
}
int ilde(){
    int a;
    while(1){
         PEND_SV();
    }
    return 0;
}

int create(int  (*procaddr)(),  /* procedure address        */
              int  ssize,      /* stack size in bytes      */
              int  priority,   /* process priority > 0     */
              char *name,      /* name (for debugging)     */
              int  nargs,      /* number of args that follow   */
              ...) {
  

  va_list ap;
  uint32 mask=irq_disable();
  int pid=newpid();
  task_t *prptr;


  char *stack = malloc(ssize);//stack_space + (index * CMCM_STACK_SIZE);
  memset(stack, 0, ssize);

  if(stack == NULL){
     //kprintf("create error malloc %s\n",name);
      panic("create error malloc\n");
      while(1);
  }
  
  prptr = &task[pid];
  prptr->elf=FALSE;
  prptr->state = PR_SUSP;
  prptr->prstkbase=stack;
  prptr->prstklen=ssize;
  strcpy(prptr->name,name);
  prptr->pid=pid;

  //ntask++;
  va_start(ap, nargs);
  for (int j=0; j < nargs; j++){
     prptr->parg[j] = (void *)va_arg(ap, long);
  }
  va_end(ap);
  // set sp to the start of the stack (highest address)
  task[pid].sp = ((uint8 *) stack) + ssize;

  task[pid].sp -= sizeof(cmcm_stack_frame_t);
  cmcm_stack_frame_t *frame = (cmcm_stack_frame_t *)task[pid].sp;

  frame->hw_frame.r0 = nargs;
  frame->hw_frame.r1 = &prptr->parg[0];
  frame->hw_frame.lr = killer;
  frame->hw_frame.pc = procaddr;
  frame->hw_frame.psr = 0x01000000; // default PSR value

  //task[pid].state |= CMCM_TASK_INUSE;
  irq_restore(mask);
  return pid;
}


void create_task(){
	//__syscall(10,33);
/*
preempt = QUANTUM;
ready_preemptive=0;
active_task = NULL;
queue_init(&ready_queue);
queue_init(&sleep_queue);
sem_init(&uart_tx,1);

for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
     task[i].state = PR_FREE;
     task[i].elf=FALSE;
     task[i].img=NULL;
}


RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
TIM2->PSC = 7;
TIM2->ARR = 10000;
TIM2->DIER |= (1 << 0);
TIM2->CR1 |= (1 << 0);
NVIC_SetPriority(TIM2_IRQn, 2); // Priority level 2
NVIC_EnableIRQ(TIM2_IRQn);

int pid=create(NULL, 2048, 1, "nullp", 0);
active_task =& task[pid];
active_task->state=PR_CURR;
//ready(create(ilde, 2048, 3, "ilde", 0));

kprintf ("Build date: %s %s %s\n\n", __DATE__, __TIME__,__FILE__);
//ready(create(ilde,STACK_SIZE,1,"null0",0));
ready(create(ilde2,STACK_SIZE,1,"null1",0)); 
kprintf("waiting.......\n");


//__syscall(10,33);
SysTick->LOAD = STK_LOAD_RELOAD;                                         // max value
SysTick->VAL  = 0;                                                       // reset
SysTick->CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE; // enable at AHB == 96MHz
ready_preemptive=1;
irq_enable();*/
while(1);
}















