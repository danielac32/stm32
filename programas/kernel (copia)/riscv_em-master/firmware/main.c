#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "kmalloc.h"



#define bool uint8_t
#define false 0
#define true (!false)



 uint32_t stack[32];
 uint32_t stack2[32];
 void *pointer;
 void *pointer2;
 uint32_t *a,*b;


extern int __strlen(char *str);
extern void save_context_task(uint32_t p);
extern void restore_context_task(uint32_t p);
extern void save_context(uint32_t a);
extern void restore_context(uint32_t b);
extern void ctxtswitch(uint32_t *spcur,uint32_t *spnext);

#define MOS_MERGE_(a,b)       a##b
#define MOS_LABEL_(line)      MOS_MERGE_(LBL, line)

#define MOS_Break(tcb){                           \
        tcb = &&MOS_LABEL_(__LINE__);             \
        return 0;                                 \
}                                                 \
MOS_LABEL_(__LINE__):                             

#define MOS_Continue(tcb) {     \
  if(tcb != NULL) {             \
     goto *tcb;                 \
  }                             \
}

/*********************************************************************/

/*********************************************************************/
int (*fun[4])();

int task2(){
  MOS_Continue(pointer2);
  static uint32_t s=0;
	for(;;){  
          
          for (int i = 0; i < 100; i+=10){
          	   myprintf("\ttask 2: %i , %i\n",i,s);
          	   s++;
               MOS_Break(pointer2);
              
          } 
	}
}

int task(){
    
   MOS_Continue(pointer);

	for(;;){  
          for (int i2 = 0; i2 < 100; ++i2){
          	   myprintf("task 1: %i",i2);
                 
               MOS_Break(pointer);
                
          }   
	}
}


 
 
int main(void)
{
  uint32_t i = 0;
  uint32_t val=0,val2=0;
  myprintf("riscv32 cpu\n");
  memset(stack,0,sizeof(stack));
  memset(stack2,0,sizeof(stack2));
  myprintf("%l---%l\n",(uint32_t)stack,(uint32_t)stack2);

  fun[0]=task;
  fun[1]=task2;
  val=100;

 while(1){ 
    





   fun[i]();
   save_context(val);




    restore_context(700);
    i++;
    if(i==1)val=100;
    if(i==2){
       val=300;
       i=0;
    }   
    //myprintf("%i,%l\n",i,val);
    save_context(700);
   // restore_context(700);
    restore_context(val);

    /*asm volatile(
               "mv a0,%0  \n\t" : 
               "=r" (val)
    );
    asm volatile("jal save_context");

    asm volatile(
               "mv a0,%0  \n\t" : 
               "=r" (val)
    );
    asm volatile("jal restore_context");*/



    /*task();

    save_context(100);
    restore_context(300);

    task2();

    save_context(300);
    restore_context(100);*/

 }
 return 0;
}
 