#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <queue.h>
#include <process.h>
#include <kernel.h>
#if 1

/*
void StkInitialize(PIC_STACK *Stack)
{
    Stack->front = -1;
    Stack->rear = -1;
    for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i)
    {
        Stack->Entries[i] = malloc(sizeof(task_t*));
    }
}

void StkPush(PIC_STACK *Stack, void *Data)
{
    if (Stack->front == -1) Stack->front = 0;
    Stack->rear = (Stack->rear + 1) % MAX_NUMBER_OF_TASKS;
    Stack->Entries[Stack->rear] = Data;
}

void *StkPop(PIC_STACK *Stack)
{
    void *data=Stack->Entries[Stack->front];
    
    if (Stack->front == Stack->rear) {
        Stack->front = -1;
        Stack->rear = -1;
    } 
    // Q has only one element, so we reset the 
    // queue after dequeing it. ?
    else {
      Stack->front = (Stack->front + 1) % MAX_NUMBER_OF_TASKS;
    }

    //Return the value that was there
    return data;
}

void *StkPeek(PIC_STACK *Stack){
  return Stack->Entries[Stack->front];
}
*/


void queue_init(queue_t *q) {
  q->first = 0;
  q->last = 0;
}

 void queue_push(queue_t *q, task_t *t) {
  if (q->first == 0) {
    t->next = 0;
    q->first = t;
    q->last = t;
  } else {
    t->next = 0;
    q->last->next = t;
    q->last = t;
  }
}

 void queue_pushsort(queue_t *q, task_t *t) {
  task_t *tmp;

  if (q->first == 0) {
    q->first = t;
    q->last = t;
    t->next = 0;
  } else {
    tmp = q->first;
    if (t->param < tmp->param) {
      t->next = tmp;
      q->first = t;
    } else {
      while (tmp->next && (t->param > tmp->next->param))
        tmp = tmp->next;

      if (tmp->next == 0) {
        tmp->next = t;
        t->next = 0;
        q->last = t;
      } else {
        t->next = tmp->next;
        tmp->next = t;
      }
    }
  }
}

 task_t* queue_pop(queue_t *q) {
  task_t *tmp = q->first;
  if (!tmp)
    return 0;

  q->first = tmp->next;
  if (!q->first)
    q->last = 0;

  return tmp;
}

 task_t* queue_peek(queue_t *q) {
  return q->first;
}
uint32 sem_init(sem_t *sem, uint32 value) {
  if (!sem)
    return ERR_PARAMS;

  sem->value = value;
  queue_init(&sem->waitq);
  return ERR_OK;
}

uint32 sem_wait(sem_t *sem) {
  if (!sem)
    return ERR_PARAMS;
  if(active_task==NULL)
    return ERR_PARAMS;
  SYS_ENTRY();
  if (sem->value > 0) {
    sem->value -= 1;
  } else {
    active_task->state = PR_WAIT;
    queue_push(&(sem->waitq), active_task);
    PEND_SV();
  }
  SYS_EXIT();
  return ERR_OK;
}

uint32 sem_signal(sem_t *sem) {
  task_t *tmp;

  if (!sem)
    return ERR_PARAMS;
  if(active_task==NULL)
    return ERR_PARAMS;
  SYS_ENTRY();
  tmp = queue_pop(&(sem->waitq));
  if (tmp) {
    tmp->state = PR_READY;
    queue_push(&ready_queue, tmp);
    PEND_SV();
  } else {
    sem->value += 1;
  }
  SYS_EXIT();
  return ERR_OK;
}


#if 0
uint32 sem_signaln(sem_t *sem,int value){
  task_t *tmp;

  if (!sem)
    return ERR_PARAMS;

  SYS_ENTRY();

  while (value--)
  {
    tmp = queue_pop(&(sem->waitq));
    if (tmp) {
      tmp->state = PR_CURR;
      queue_push(&ready_queue, tmp);
    } 
  }

  SYS_EXIT();
  return ERR_OK;
}

uint32 sem_count(sem_t *sem){
  return sem->value;
}
#endif
#endif