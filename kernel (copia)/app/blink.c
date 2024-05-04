

#include <stdarg.h>
#include "lib.h"

extern void _fdoprnt(char *, va_list, int (*)(int, char), int);

 __attribute__((naked))
void xinu_syscall(int nr, int arg1, int arg2, int arg3) 
{
    asm volatile("push {r0-r11}");
    asm volatile("push {lr}");
    asm volatile("svc 1"); 
    asm volatile("pop {lr}");
    asm volatile("pop {r0-r11}");
    asm volatile("bx lr");
}

int xinu_putc(int dev, char c) {
    xinu_syscall(XINU_PUTC, dev, c, 0 );
    return 0;
}


int __printf(
      const char        *fmt,
      ...
    )
{
    va_list ap;

    va_start(ap, fmt);
    _fdoprnt((char *)fmt, ap, xinu_putc, 0);
    va_end(ap);

    return 0;
}



/*
void puts(char *str){
  while(*str){
        __syscall(XINU_PUTC,0,*str++);
  }
}*/
//char str[100];
//char *buff="11111111111";
int y=0xcafe;

int main(){

  //int y=999;
    if(y == 0xdd){
        return y;
    }
      
    __printf("iniciando programa \n");
   // puts("iniciando\n");
    //printf2("iniciando app...\n");

   __syscall(XINU_PIN_MODE,GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

   
    while(1){
    __syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 1);
    __syscall(XINU_SLEEP,200);
    __syscall(XINU_PIN_SET,GPIOx(GPIO_C),13, 0);
    __syscall(XINU_SLEEP,100);
    //printf2("loop blink\n");
      //puts("blink\n");
    
    //}
  }
return 8888;
}


 