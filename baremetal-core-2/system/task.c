#include <xinu.h>

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


int getpid(){
  return active_task->pid;
}

void ready(int pid){
task_t *prptr;
prptr =& task[pid];
prptr->state = PR_CURR;
queue_push(&ready_queue, prptr); 
}

void sleep(uint32 ticks) {
uint32 mask=disable();
  if (ticks) {
      active_task->state = PR_SLEEP;
      active_task->param = ticks*1000 +sys_time;//*1000;
      queue_push(&sleep_queue, active_task);
      //queue_push(&sleep_queue, active_task);
  }
  restore(mask);
  PEND_SV();
}

void kill(int pid){

  uint32 mask=disable();

  task_t *prptr;
  prptr =& task[pid];
  

  free(prptr->prstkbase);

  if(prptr->elf == TRUE){
     prptr->elf = FALSE;
     free(prptr->img);//free(prptr->img);
  }


  switch(prptr->state){
        case PR_CURR:
             prptr->state=PR_FREE;
             kprintf("killed  %d--%s\n", pid,prptr->name);
             restore(mask);
             PEND_SV();
        break;
        case PR_SLEEP:
             {

              //kprintf("\n");
              task_t *tmp = queue_peek(&sleep_queue);
              while (tmp->state==PR_SLEEP) {
                    tmp = queue_pop(&sleep_queue);

                    if (tmp->pid==prptr->pid) {
                        //tmp = queue_pop(&sleep_queue);
                        tmp->state = PR_FREE;
                        kprintf("killed  %d--%s\n", tmp->pid,tmp->name);
                        break;
                    }else{
                        queue_push(&sleep_queue, tmp);
                    }
                    
                    //queue_push(&sleep_queue, tmp);
              }

              /*task_t *tmp = queue_peek(&sleep_queue);
              while (tmp) {
                if (tmp->pid==prptr->pid) {
                    tmp = queue_pop(&sleep_queue);
                    tmp->state = PR_FREE;
                    kprintf("killed  %d--%s\n", pid,prptr->name);
                    break;
                }
              }*/
              
             }
        break;
        default:
            prptr->state = PR_FREE;
            //restore(mask);
  }
  restore(mask);
  //PEND_SV();
}



local void killer(){
      int pid = getpid();
      kill(pid);
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



#if 1
void __attribute__((naked)) pendsv_handler()  {
//cmcm_context_switch();
  // the first context switch will be called from the MSP
  // in that case we do not need to save the context
  // since we never return to MSP

  
  //if (current_task != -1) {
    //  task[current_task].sp = cmcm_push_context();
  //}


   if (active_task != NULL){
       active_task->sp = cmcm_push_context();
   }

   if (active_task->state == PR_CURR  && active_task->state != PR_FREE) {
      queue_push(&ready_queue, active_task);
   }

   active_task = queue_pop(&ready_queue);
   preempt = QUANTUM;

  // find next running task
  /*uint8 running;
  do {
    current_task++;
    if (current_task >= CMCM_MAX_NUM_TASKS) {
      current_task = 0;
    }

    uint8 state = task[current_task].state;
    running = (state & CMCM_TASK_INUSE) & !(state & CMCM_TASK_SLEEPING);
  } while (!running);*/

  cmcm_pop_context(active_task->sp);

  __asm__("bx %0" : : "r" (0xFFFFFFFD));
}


void SysTick_Handler(void){
sys_time += 1;
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





/*
typedef struct {
  void *sp;
  uint8 state;
} cmcm_task;*/



//static cmcm_task task[CMCM_MAX_NUM_TASKS];
//static uint8 __attribute__((aligned(8))) stack_space[CMCM_STACK_SIZE * CMCM_MAX_NUM_TASKS];

// the first context switch (from MSP to PSP) will increment this
static int current_task = -1;


 
int create(int  (*procaddr)(),  /* procedure address        */
              int  ssize,      /* stack size in bytes      */
              int  priority,   /* process priority > 0     */
              char *name,      /* name (for debugging)     */
              int  nargs,      /* number of args that follow   */
              ...) {
  

  va_list ap;
  uint32 mask=disable();
  int pid=newpid();
  task_t *prptr;

  //int index;
  //for (index = 0; task[index].state & CMCM_TASK_INUSE && index < CMCM_MAX_NUM_TASKS; index++);
  //if (index >= CMCM_MAX_NUM_TASKS) {
    //return;
  //}

  void *stack = (void *)malloc(ssize);//stack_space + (index * CMCM_STACK_SIZE);
  memset(stack, 0, ssize);

  if(stack == NULL){
     kprintf("create error malloc %s\n",name);
      while(1);
  }
  
  prptr = &task[pid];
  prptr->elf=FALSE;
  prptr->state = PR_SUSP;
  prptr->prstkbase=stack;
  prptr->prstklen=ssize;
  strcpy(prptr->name,name);
  prptr->pid=pid;

  ntask++;
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
  restore(mask);
  return pid;
}



#endif