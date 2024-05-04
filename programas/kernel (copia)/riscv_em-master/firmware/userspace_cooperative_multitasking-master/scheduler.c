#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#include "sc-collections.h"

struct task {
	/*
	 * Record the status of this task
	 */
	enum {
		ST_CREATED,
		ST_RUNNING,
		ST_WAITING,
	} status;

	int id;

	/*
	 * For tasks in the ST_RUNNING state, this is where to longjmp back to
	 * in order to resume their execution.
	 */
	jmp_buf buf;

	/*
	 * Function and argument to call on startup.
	 */
	void (*func)(void*);
	void *arg;

	/*
	 * Link all tasks together for the scheduler.
	 */
	struct sc_list_head task_list;

	void *stack_bottom;
	void *stack_top;
	int stack_size;
};

enum {
	INIT=0,
	SCHEDULE,
	EXIT_TASK,
};

struct scheduler_private {
	/*
	 * Where to jump back to perform scheduler actions
	 */
	jmp_buf buf;

	/*
	 * The current task
	 */
	struct task *current;

	/*
	 * A list of all tasks.
	 */
	struct sc_list_head task_list;
} priv;

void scheduler_init(void)
{
	priv.current = NULL;
	sc_list_init(&priv.task_list);
}

void scheduler_create_task(void (*func)(void *), void *arg)
{
	static int id = 1;
	struct task *task = malloc(sizeof(*task));
	task->status = ST_CREATED;
	task->func = func;
	task->arg = arg;
	task->id = id++;
	task->stack_size = 16 * 1024;
	task->stack_bottom = malloc(task->stack_size);
	task->stack_top = task->stack_bottom + task->stack_size;
	sc_list_insert_end(&priv.task_list, &task->task_list);
}

void scheduler_exit_current_task(void)
{
	struct task *task = priv.current;
	/* Remove so we don't schedule this again */
	sc_list_remove(&task->task_list);
	/* Would love to free the task... but if we do, we won't have a
	 * stack anymore, which would really put a damper on things.
	 * Let's defer that until we longjmp back into the old stack */
	longjmp(priv.buf, EXIT_TASK);
	/* NO RETURN */
}

static struct task *scheduler_choose_task(void)
{
	struct task *task;

	sc_list_for_each_entry(task, &priv.task_list, task_list, struct task)
	{
		if (task->status == ST_RUNNING || task->status == ST_CREATED) {
			/* We'll pick this one, but first we should move it to
			 * the back to ensure that we pick a new task next
			 * time. Note that this is only safe because we're
			 * exiting the loop now. If we continued looping we
			 * could have trouble due to modifying the linked list.
			 */
			sc_list_remove(&task->task_list);
			sc_list_insert_end(&priv.task_list, &task->task_list);
			return task;
		}
	}

	return NULL;
}

static void schedule(void)
{
	struct task *next = scheduler_choose_task();

	if (!next) {
		return;
	}

	priv.current = next;
	if (next->status == ST_CREATED) {
		/*
		 * This task has not been started yet. Assign a new stack
		 * pointer, run the task, and exit it at the end.
		 */
		register void *top = next->stack_top;
		asm volatile(
			"mov %[rs], %%rsp \n"
			: [ rs ] "+r" (top) ::
		);

		/*
		 * Run the task function
		 */
		next->status = ST_RUNNING;
		next->func(next->arg);

		/*
		 * The stack pointer should be back where we set it. Returning would be
		 * a very, very bad idea. Let's instead exit
		 */
		scheduler_exit_current_task();
	} else {
		longjmp(next->buf, 1);
	}
	/* NO RETURN */
}

void scheduler_relinquish(void)
{
	if (setjmp(priv.current->buf)) {
		return;
	} else {
		longjmp(priv.buf, SCHEDULE);
	}
}

static void scheduler_free_current_task(void)
{
	struct task *task = priv.current;
	priv.current = NULL;
	free(task->stack_bottom);
	free(task);
}


void scheduler_run(void)
{
	/* This is the exit path for the scheduler! */
	switch (setjmp(priv.buf)) {
	case EXIT_TASK:
		scheduler_free_current_task();
	case INIT:
	case SCHEDULE:
		schedule();
		/* if return, there's nothing else to do and we exit */
		return;
	default:
		fprintf(stderr, "Uh oh, scheduler error\n");
		return;
	}
}
