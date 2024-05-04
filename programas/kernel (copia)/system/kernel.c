#include <os.h>

 uint32 sys_time;
 task_t *active_task;
 queue_t ready_queue;
 queue_t sleep_queue;
 task_t task[MAX_NUMBER_OF_TASKS];
 int ntask;
 uint8_t preempt;

local int   newpid(void)
{
    uint32_t  i;          /* iterate through all processes*/
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

void load_context(void* psp) {
  asm volatile(
  "mov sp, %0   \n"
  "pop  {r4-r7} \n"
  "mov  r8, r4  \n"
  "mov  r9, r5  \n"
  "mov  r10, r6 \n"
  "mov  r11, r7 \n"
  "pop  {r4-r7} \n"
  "bx %1      \n" : : "r" (psp), "r" (0xFFFFFFF9)
  );
}

void* save_context() {
  void *reg;
  asm volatile(
  "push {r4-r7} \n"
  "mov  r4, r8 \n"
  "mov  r5, r9 \n"
  "mov  r6, r10 \n"
  "mov  r7, r11 \n"
  "push {r4-r7} \n"
  "mov %0, sp \n" : "=r" (reg)
  );
  return reg;
}



void __attribute__((naked)) pendsv_handler()  {
  void* reg;
  reg = save_context();
  active_task->sp = reg;

  if (active_task != 0 && active_task->state == PR_CURR) {
      queue_push(&ready_queue, active_task);
  }
  active_task = queue_pop(&ready_queue);
  preempt = QUANTUM;
  load_context(active_task->sp);
}

void systick_handler() {
  sys_time += 1;
  task_t *tmp = queue_peek(&sleep_queue);
  while (tmp) {
    if (sys_time >= tmp->param) {
      tmp = queue_pop(&sleep_queue);
      tmp->state = PR_CURR;
      queue_push(&ready_queue, tmp);
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

 
void initialice(){
  int i,j;
  sys_time = 0;
  active_task = 0;
  for (i = 0; i < MAX_NUMBER_OF_TASKS; ++i){
       task[i].state = PR_FREE;
       for (j = 3; j < NDESC; j++)
        {
            task[i].fdesc[j] = UNUSED;  // UNSED value declared in the FS.h
        }
  }
  
  preempt=QUANTUM;
  ntask=0;
  
  for (int i = 0; i < 3; ++i){
        init(i);
  }
  
  struct  dentry  *devptr;        

  /*printf("Device     Name     Minor\n");
  printf("------ ------------ -----\n");

  for (i = 0; i < 3; i++) {
    devptr = &devtab[i];
    printf("%4d   %-12s %3d\n", i, devptr->dvname,devptr->dvminor);
  }
  printf("\n");*/
 // memset(bs2,'a',1000);
 
  //hexdump(bs2,1000,true);
  queue_init(&ready_queue);
  queue_init(&sleep_queue);

}


uint32_t *prepare_task_stack(uint32_t *task_stack,uint32_t size,void *entry, uint32_t nargs, void *parg){
  task_stack += size - sizeof(context_t); /* End of task_stack, minus what we are about to push */
  context_t *ctx = (context_t*)task_stack;

  ctx->r0 = (uint32_t) nargs;
  ctx->r1 = (uint32_t) parg;
  ctx->lr = (uint32_t) thread_terminate;
  ctx->pc = (uint32_t) entry;
  ctx->psr = (uint32_t) 0x01000000; /* PSR Thumb bit */
  return task_stack;
}



int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    ){
va_list ap;
uint32_t mask=disable();
int pid=newpid();//getNextPID();
task_t *prptr;

uint32_t *saddr=(uint32_t*)malloc(ssize);/*getmem(ssize);//newStack;; */
//uint32_t *saddr=(uint32_t*)getmem(ssize);//newStack;; */

if(saddr==(uint32_t *)SYSERR){
   printf("error malloc %s\n",name);
   while(1);
}
prptr = &task[pid];
prptr->state = PR_SUSP;
prptr->wait_ticks=0;
prptr->prstkbase=saddr;
prptr->prstklen=ssize;
strcpy(prptr->name,name);
prptr->pargs = nargs;
prptr->paddr = (int *)procaddr;
prptr->pid=pid;
ntask++;

va_start(ap, nargs);
for (int j=0; j < nargs; j++){
     prptr->parg[j] = (void *)va_arg(ap, long);
}
va_end(ap);
prptr->sp =  prepare_task_stack(saddr,ssize,procaddr,nargs,&prptr->parg[0]);
restore(mask);
printf("pid: %d\n",pid );
return pid;
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
    active_task->param = ticks + sys_time;
    queue_pushsort(&sleep_queue, active_task);
  }
  PEND_SV();
restore(mask);
}


void kill(int pid){
  /*SYS_ENTRY();
  task[pid].state = PR_FREE;
  freemem((char *)task[pid].prstkbase,task[pid].prstklen);
  PEND_SV();
  SYS_EXIT();*/
  uint32 mask=disable();
  task_t *prptr;
  prptr =& task[pid];
  ///freemem((char *)prptr->prstkbase,prptr->prstklen);
  free((char *)prptr->prstkbase);
  switch(prptr->state){
        case PR_CURR:
             prptr->state=PR_FREE;
             restore(mask);
             PEND_SV();
        break;
        case PR_SLEEP:
             {
              task_t *tmp = queue_peek(&sleep_queue);
              while (tmp) {
                if (tmp->pid==prptr->pid) {
                    tmp = queue_pop(&sleep_queue);
                    tmp->state = PR_FREE;
                    printf("killed  %d--%s\n", pid,prptr->name);
                    break;
                }
              }
              restore(mask);
              PEND_SV();
             }
        break;
        default:
            prptr->state = PR_FREE;
  }
  restore(mask);
}


void thread_terminate() {
  /*SYS_ENTRY();
  active_task->state = PR_FREE;
  freemem((char *)active_task->prstkbase,active_task->prstklen);
  PEND_SV();
  SYS_EXIT();*/
  uint32 mask=disable();
 // printf("exit\n");
  //freemem((char *)active_task->prstkbase,active_task->prstklen);
  free((char *)active_task->prstkbase);
  switch(active_task->state){
        case PR_CURR:
             active_task->state=PR_FREE;
             restore(mask);
             PEND_SV();
        break;
        case PR_SLEEP:
             {
              task_t *tmp = queue_peek(&sleep_queue);
              while (tmp) {
                if (tmp->pid==active_task->pid) {
                    tmp = queue_pop(&sleep_queue);
                    tmp->state = PR_FREE;
                    printf("killed  %d--%s\n", active_task->pid,active_task->name);
                    break;
                }
              }
              restore(mask);
              PEND_SV();
             }
        break;
        default:
            active_task->state = PR_FREE;
  }
  restore(mask);


}