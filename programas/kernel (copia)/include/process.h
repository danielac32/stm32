#include "kernel.h"


#ifndef PROCESS_H
#define PROCESS_H
#define MAX_NUMBER_OF_TASKS 12

typedef struct {
  uint32 r4, r5, r6, r7, r8, r9, r10, r11;
  uint32 r0, r1, r2, r3, r12, lr, pc, psr;
} context_t;

#define STACK_SIZE    4096*2

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

typedef struct task_t
{
    uint32_t  *prstkptr;  /* Saved stack pointer      */
    uint32_t  *prstkbase; /* Base of run time stack   */
    uint32_t  *sp;
    uint32_t  prstklen; /* Stack length in bytes    */
    int pargs;              
    void *parg[MAX_ARG]; 
    int *paddr;
    int pid;
    uint8_t prio;
    uint16_t state;
    uint32_t wait_ticks;
    uint32_t param;
    uint8_t name[16];
    int fdesc[NDESC]; 
    struct task_t *next;
}task_t;



extern uint8_t preempt;
extern int ntask;
extern task_t *active_task;
extern task_t task[];
extern void kill(int);
extern void ready(int);
extern void delay(uint32);
extern void panic();
extern void initialice();
extern void sleep(uint32);
extern int getpid();
extern void thread_terminate();

#endif