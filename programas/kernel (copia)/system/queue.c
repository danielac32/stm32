#include <os.h>


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
uint sem_init(sem_t *sem, uint value) {
  if (!sem)
    return ERR_PARAMS;

  sem->value = value;
  queue_init(&sem->waitq);
  return ERR_OK;
}

uint sem_wait(sem_t *sem) {
  if (!sem)
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

uint sem_signal(sem_t *sem) {
  task_t *tmp;

  if (!sem)
    return ERR_PARAMS;

  SYS_ENTRY();
  tmp = queue_pop(&(sem->waitq));
  if (tmp) {
    tmp->state = PR_CURR;
    queue_push(&ready_queue, tmp);
    PEND_SV();
  } else {
    sem->value += 1;
  }
  SYS_EXIT();
  return ERR_OK;
}

