
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include <kernel.h>


typedef struct task_t
{
    //uint32  *prstkptr;  /* Saved stack pointer      */
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
    bool8   elf;
    void *img;
    
    struct task_t *next;
}task_t;


int main(int argc, char const *argv[])
{
	int32  retval;         /* return value         */
    int pid;            /* ID of process to kill    */
    pid = atoi(argv[1]);
    //printf("kill: %d\n",pid );
    if (pid == 0) {
        printf("%s: cannot kill the null process\n",
            argv[0]);
        return 1;
    }

    task_t *prptr = (task_t *)syscall_struct_task(pid);
    if(prptr->state!=PR_FREE){
       syscall_kill(pid);
    }else{
       printf("not found task %d\n",pid);
    }
    printf("killed\n");
    return 0;
	return 0;
}