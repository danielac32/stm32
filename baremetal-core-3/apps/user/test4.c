#include <string.h>
#include <stdio.h>
#include <syscall.h>
#include <gpio.h>



#define	DELAY(n)                                	\
{                                               	\
	volatile long N = 2*n;				\
							\
	while(N > 0) {					\
		N--;					\
	}						\
}

/*
 * Delay units are in milli-seconds.
 */
#define	MDELAY(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		DELAY(1000);				\
	}						\
}


int main(int argc, char *argv[])
{

   uint32 r;
   bool s=false;
   if(!strcmp(argv[1],"syscall")){
       s=true;
   }
   for (int i = 0; i < 1000; ++i)
   {
   	   r += i;
   	   if(!s){
   	   	MDELAY(500);
   	   }
   	   else{
   	   	syscall_sleep(100);
   	   }
   }

   printf("result %d\n", r);
   return r;
}