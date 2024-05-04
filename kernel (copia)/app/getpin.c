

#include <stdarg.h>
#include "lib.h"

extern void _fdoprnt(char *, va_list, int (*)(int, char), int);

 __attribute__((naked))
void xinu_syscall(int nr, int arg1, int arg2, int arg3) 
{
    asm volatile("push {r0-r11}");
    asm volatile("push {lr}");
    asm volatile("svc 1"); /* Hack: For all service call use 1, arg1 got number */
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





int main(){

 
    __printf("iniciando programa2 \n");
   // puts("iniciando\n");
    //printf2("iniciando app...\n");

   __syscall(XINU_PIN_MODE,GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

   
    while(1){

    	if(!__syscall(XINU_PIN_GET,GPIOx(GPIO_A),0)){
			__printf("key button ok\n");
			__syscall(XINU_SLEEP,200);
		}
  }
return 0;
}


