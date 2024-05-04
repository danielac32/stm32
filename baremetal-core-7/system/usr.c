#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <usbapi.h>
#include <queue.h>
#include <memory.h>
#include <kernel.h>
#include <process.h>
#include <syscall.h>

#include "st7735.h"
#include "gfx.h"

//int len;


int fontWidth = 6;
unsigned int fontHeight = 8;

unsigned int termWidth, termHeight;

unsigned int termCursX = 0;
unsigned int termCursY = 0;

uint16_t termFgColor = ST77XX_WHITE;
uint16_t termBgColor = ST77XX_BLACK;

void blink(){
    while(1){
       hw_set_pin(GPIOx(GPIO_C),13, 0);
       sleep(100);
       hw_set_pin(GPIOx(GPIO_C),13, 1);
       sleep(1000);
    }
}
 
int test1(){
    int a;
    while(1){
       printf("task2 %d\n",a);
      //PEND_SV();//
       a++;
       sleep(1000);
      
    }
    return 0;
}



void shell();

int nullp(){
    ready_preemptive=1;
    //ready(create((void *)blink, 2048, 3, "blink", 0));
    //ready(create(test1,STACK_SIZE,1,"test1",0));

    
    ready(create((void *)shell,STACK_SIZE,1,"shell",0));
    syscall_init(&syscallp);
    //uint32 r=__syscall(10,33);
   // printf("%08x\n",r );

    //uint32 free= syscallp.freeHeap();
    //printf("memory: %d\n",free );

    //syscall(XINU_NULLPROCESS);
    

    while(1){

        
        //printf("nullp\n");
        sleep(2000);
        //PEND_SV();
    }
    return 0;
}