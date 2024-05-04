#include <stdlib.h>
#include <interrupt.h>
#include <process.h>
#include <queue.h>
#include <memory.h>
#include <stdio.h>
#include <stddef.h>
#include <gpio.h>
#include <syscall.h>


queue_t ready_queue;
queue_t sleep_queue;
task_t *active_task=NULL;
uint32 sys_time;
uint32 preempt;

task_t task[MAX_NUMBER_OF_TASKS];


static int   newpid(void)
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

void sleep(uint32 ticks) {
  uint32 mask=irq_disable();
  if (ticks) {
      active_task->state = PR_SLEEP;
      active_task->param = ticks + sys_time;//*1000;
      queue_pushsort(&sleep_queue, active_task);
      //queue_push(&sleep_queue, active_task);
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
             //kprintf("curr .killed  %d--%s\n", prptr->pid,prptr->name);
        break;
        case PR_SLEEP:
             {
                  task_t *tmp = queue_peek(&sleep_queue);
                  while (tmp->state==PR_SLEEP) {
                        tmp = queue_pop(&sleep_queue);

                        if (tmp->pid==prptr->pid) {
                            //tmp = queue_pop(&sleep_queue);
                            tmp->state = PR_FREE;
                            //kprintf("sleep .killed  %d--%s\n", tmp->pid,tmp->name);
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
            
            //kprintf("default .killed  %d--%s \n", prptr->pid,prptr->name);
            prptr->state = PR_FREE;
            break;
            
  }
  irq_restore(mask);
}


static void killer(){
      int pid = getpid();
      kill(pid);
      //kprintf("killed %s\n",task[pid].name);
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

void  __attribute__((naked)) PendSV_Handler(void){
   
 uint32 * restorestk=cmcm_push_context();
 
 task_t *ptold;     
 task_t *ptnew; 

 ptold = active_task;
 if (ptold->state == PR_CURR) { 
     ptold->state = PR_READY;
     queue_push(&ready_queue, ptold);
 }
 active_task = queue_pop(&ready_queue);
 ptnew = active_task;
 active_task->state=PR_CURR;
 preempt = QUANTUM;
 ptold->sp = restorestk;
 
 cmcm_pop_context(ptnew->sp);
  __asm__("bx %0" : : "r" (0xFFFFFFFD));
}


void ready(int pid/*, bool resch*/)
{
    task_t *prptr;
    prptr =& task[pid];
    prptr->state = PR_CURR;
    queue_push(&ready_queue, prptr); 
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


  void *stack = (void *)malloc(ssize);//stack_space + (index * CMCM_STACK_SIZE);
  memset(stack, 0, ssize);

  if(stack == NULL){
     //kprintf("create error malloc %s\n",name);
      kpanic("create error malloc\n");
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

  irq_restore(mask);
  return pid;
}




int ilde3(){
    while(1){
       //kprintf("ilde3 running..\n");
       //sleep(2000);
    }
}

int ilde2(){
    while(1){
       //kprintf("ilde running..\n");
       sleep(1000);
    }
}

int ilde(){
	kprintf("ilde running..\n");
	while(1){
	   hw_toggle_pin(GPIOx(GPIO_C),13);
       sleep(500);

       //syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 0);
      // syscall(XINU_SLEEP,100);
       //syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 1);
       //syscall(XINU_SLEEP,1000);
	}
}

extern void usr();

int nullp(){
	ready(create((void *)ilde,512,1,"blink",0));
    //ready(create((void *)ilde2,1024,1,"blink2",0));
    //ready(create((void *)ilde3,1024,1,"blink3",0));
	ready(create((void *)usr,STACK_SIZE,1,"usr",0));
    kprintf("ilde nullp..\n");
	//while(1){
	  //  PEND_SV();
	//}
    return 0;
}


void init_kernel(){

preempt = QUANTUM;
active_task = NULL;
queue_init(&ready_queue);
queue_init(&sleep_queue);
//sem_init(&uart_tx,1);
sem_init(&sem_gpio,1);

for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
     task[i].state = PR_FREE;
     task[i].elf=FALSE;
     task[i].img=NULL;
}
clkinit();

int pid=create(nullp, STACK_SIZE/2, 1, "nullp", 0);
active_task =& task[pid];
active_task->state=PR_CURR;
kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
irq_enable();
TIM2->CR1 |= (1 << 0);
ready_preemptive=1;
asm volatile ("mov r0, %0\n" : : "r" (active_task->sp));
asm volatile ("msr psp, r0");
asm volatile ("ldmia r0!, {r4-r11} ");
asm volatile ("msr psp, r0");
asm volatile ("mov r0, #2");
asm volatile ("msr control, r0");
asm volatile ("isb");
nullp();

for(;;);
}