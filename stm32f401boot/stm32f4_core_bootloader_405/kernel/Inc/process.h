#include <stdint.h>
#include <stdbool.h>
#include <kernel.h>

//#include "queue.h"

#ifndef PROCESS_H
#define PROCESS_H


#define THREAD_PSP  0xFFFFFFFD
#define SCB_ICSR  (uint32 *) 0xE000ED04
#define PENDSV_INTR 28


#define MAX_NUMBER_OF_TASKS 12
#define NTHREAD   12  

typedef struct {
  uint32 r4, r5, r6, r7, r8, r9, r10, r11;
  uint32 r0, r1, r2, r3, r12, lr, pc, psr;
} context_t;

typedef struct {
  // we explicity push these registers
  struct {
    uint32 r4;
    uint32 r5;
    uint32 r6;
    uint32 r7;
    uint32 r8;
    uint32 r9;
    uint32 r10;
    uint32 r11;
  } sw_frame;

  // these registers are pushed by the hardware
  struct {
    uint32 r0;
    uint32 r1;
    uint32 r2;
    uint32 r3;
    uint32 r12;
    void *lr;
    void *pc;
    uint32 psr;
  } hw_frame;
} cmcm_stack_frame_t;

#define STACK_SIZE    4096

extern int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    );


#define MAX_ARG 4
#define PR_FREE   0 /* Process table entry is unused  */
#define PR_CURR   1 /* Process is currently running   */
#define PR_READY  2 /* Process is on ready queue    */
#define PR_RECV   3 /* Process waiting for message    */
#define PR_SLEEP  4 /* Process is sleeping      */
#define PR_SUSP   5 /* Process is suspended     */
#define PR_WAIT   6 /* Process is on semaphore queue  */
#define PR_RECTIM 7 /* Process is receiving with timeout  */
#define PR_KILL     8

#define NDESC  8
#define TNMLEN      16          /**< length of thread "name"            */
#define NULLTHREAD  0           /**< id of the null thread              */
#define BADTID      (-1)        /**< used when invalid tid needed       */


typedef struct task_t
{
    void  *prstkbase; /* Base of run time stack   */
    void  *sp;
    uint32  prstklen; /* Stack length in bytes    */
    //int pargs;              
    void *parg[MAX_ARG]; 
    //int *paddr;
    uint8 pid;
    uint8 prio;
    uint8 state;
    //uint32 wait_ticks;
    uint32 param;
    //sem_t sem;
    char name[16];
    bool  elf;
    void *img;
    
    struct task_t *next;
}task_t;

#define isbadtid(x) ((x)>=NTHREAD || (x)<0 || PR_FREE == thrtab[(x)].state)


extern uint32 sys_time;
extern uint32 preempt;
extern task_t task[];//extern task_t thrtab[];
extern int thrcount;            /**< currently active threads           */
extern int thrcurrent;  
extern task_t *active_task;
//extern task_t task[];
//extern sem_t sem_task[];

int getpid();
int getprio(int);
void kill(int);
void ready(int/*, bool*/);
void resched(void);
void sleep(uint32 ticks);
int yield(void);
void userret(void);
void clkhandler(void);

#define THREAD_PSP  0xFFFFFFFD

/* Thread Control Block */
/*typedef struct {
  void *stack;
  void *orig_stack;
  uint8 in_use;
} tcb_t;

tcb_t tasks[MAX_NUMBER_OF_TASKS];
int lastTask;*/


#endif