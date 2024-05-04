//#include <kernel.h>
#include <stdio.h>
#include <syscall.h>
#include <gpio.h>



void asus(int num){
for (int i = 1; i <= num; ++i)
{
	for (int j = 0; j <= num; ++j)
	{
		if( i==1 || i==num || j==1 || j == num){
             printf("* ");
		}else{
             printf("  ");
		}
	}
	printf("\n");
}
}

int main(int argc, char *argv[])
{

	printf("get pin %d\n",argc);
	for (int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
	syscall_pin_mode(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

   
    while(1){

    	if(!syscall_pin_get(GPIOx(GPIO_A),0)){
			printf("key button ok\n");
			asus(20);
			syscall_sleep(200);
		}
  }
    return 0;
}