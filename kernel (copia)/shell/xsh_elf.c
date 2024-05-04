/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <elf.h>

#define FROM_ADDR_1  0x800c000
#define FROM_ADDR_2  0x8010000

shellcmd xsh_elf(int nargs, char *args[])
{
    exec_img ximg;
    program_img point;
    int32	msg;
    int32	tmparg;

    int (*p)(int,char *);

	if(!strncmp(args[1],"1",1)){
       point.img = (void*)FROM_ADDR_1;
	}else if(!strncmp(args[1],"2",1)){
       point.img = (void*)FROM_ADDR_2;
	}
   

    int pid = getpid();
    int r = load_elf(&point, &ximg);
    //int pid =create((void *)ximg.entry, 4096, INITPRIO, "load", 1,"hola");
    struct procent * prptr = &proctab[pid];
	prptr->elf = TRUE;
	prptr->img = ximg.start;

	//if ((pid == SYSERR) ||(addargs(pid, nargs, args, nargs-2, args, &tmparg) == SYSERR) ) {
	//	 printf("error\n");
	//	 return;
	//}

    //msg = recvclr();
	//resume(pid);
    
    p = (int *)ximg.entry;
    int re = p(nargs-2,&args[2]);
    //free(ximg.start);
    
    //program_img prg1 = { .img = (void*)FROM_ADDR_1 };
    

    /*program_img point;

    point.img =(void*)FROM_ADDR_1;


    exec_img ximg;

    int r = load_elf(&point, &ximg);
    int (*p)(void)= (int *)ximg.entry;

    int re = p();
    printf("result %d\n", re);
    free(ximg.start);*/
	return 0;
}
