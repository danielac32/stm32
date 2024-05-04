
#include "kernel.h"
#include "process.h"
#ifndef QUEUE_H
#define QUEUE_H

 
#define ERR_OK      0
#define ERR_NORES   1
#define ERR_PARAMS    2

#define SYS_ENTRY()   asm volatile("cpsid i")
#define SYS_EXIT()    asm volatile("cpsie i")


typedef struct {
  task_t *first;
  task_t *last;
} queue_t;

typedef struct {
  uint32 value;
  queue_t waitq;
} sem_t;


uint32 sem_init(sem_t *sem, uint32 value);
uint32 sem_signal(sem_t *sem);
uint32 sem_wait(sem_t *sem);


 void queue_init(queue_t *q);

 void queue_push(queue_t *q, task_t *t);

 void queue_pushsort(queue_t *q, task_t *t);

 task_t* queue_pop(queue_t *q);
 task_t* queue_peek(queue_t *q);


extern queue_t ready_queue;
extern queue_t sleep_queue;

/*typedef struct {
  struct procent *first;
  struct procent *last;
}queue_t;*/

//extern  queue_t ready_queue;


//void task_enqueue(struct procent *t);
//struct procent* task_dequeue();
/*
void queue_init(queue_t *q);
void queue_push(queue_t *q, struct procent *t);
//void queue_pushsort(queue_t *q, struct procent *t);
struct procent* queue_pop(queue_t *q);
struct procent* queue_peek(queue_t *q);*/

#endif 