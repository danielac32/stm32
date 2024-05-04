
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

//char *state[]={"free ", "curr ", "ready", "recv ", "sleep", "susp ","wait "};


struct  state_t  {           /* Entry in command table   */
    char    state[10];          /* Name of command      */
};


const  struct  state_t  t_state[] = {
    {"free "},
    {"curr "},
    {"ready "}, 
    {"recv "}, 
    {"sleep "}, 
    {"susp "},
    {"wait "}
};



int main(int argc, char const *argv[])
{
	/*char *pstate[]	= {		
		"free ", "curr ", "ready", "recv ", "sleep", "susp ",
		"wait "};

    
    for (int i = 0; i < 7; ++i)
    {
        printf("-> %d--%s\n",i,state[i] );
    }*/
    printf("\n");


    printf("%3s %-16s %5s %10s %10s %10s\n",
		   "Pid", "Name", "State", "Stack Base",
		   "Stack Ptr", "Stack Size");
    

	for (int i = 0; i < syscall_num_task()/*MAX_NUMBER_OF_TASKS*/; i++) {
		task_t *prptr = (task_t *)syscall_struct_task(i);//&task[i];
		if (prptr->state == PR_FREE) {  
			continue;
		}
		printf("%3d %-16s %5s 0x%08X 0x%08X %8d\n",
			i, prptr->name, t_state[(int)prptr->state].state,
			prptr->prstkbase,(uint32)prptr->sp, prptr->prstklen);
	}

    /*printf("\n");
    for (int i = 0; i < syscall_num_task(); i++) {
        task_t *prptr = (task_t *)syscall_struct_task(i);//&task[i];
        if (prptr->state == PR_FREE) {  
            continue;
        }
        printf("%d %s %s 0x%08X 0x%08X %d\n",
            i, prptr->name, pstate[(int)prptr->state],
            prptr->prstkbase,(uint32)prptr->sp, prptr->prstklen);
    }*/
	return 0;
}