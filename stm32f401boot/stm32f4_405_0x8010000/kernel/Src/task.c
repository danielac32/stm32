#include <stdlib.h>
#include <interrupt.h>
#include <process.h>
#include <queue.h>
#include <memory.h>
#include <stdio.h>
#include <stddef.h>
#include <gpio.h>
#include <syscall.h>

extern void shell();

void usr(){
	syscall_init(&syscallp);
	ready(create((void *)shell,STACK_SIZE,1,"shell",0));
    
}