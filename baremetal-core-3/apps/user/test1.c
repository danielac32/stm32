//#include <kernel.h>
#include <stdio.h>
#include <syscall.h>
#include <gpio.h>



/*
void asus2(int n){
int c,k,space=1;
space = n -1;
for (k = 1; k <= n; k++)
{
	for (c=1;c<=space;c++)
		printf(" ");
	 
	space--;
    
    for (c=1;c<=2*(k-1);c++)
		printf("*");

    printf("\n");
}
space =1;
for (k = 1; k <= n-1; ++k)
{
	for (c=1;c<=space;c++)
		printf(" ");
	 
	space++;
    
    for (c=1;c<=2*(n-k)-1;c++)
		printf("*");

    printf("\n");
}

}
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
	printf("esta es una prueba de libreria para elf ejecutables %d\n",argc);
	for (int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
	asus(20);
	asus2(20);
	return 0;
}*/


void asus2(int n){
int c,k,space=1;
space = n -1;
for (k = 1; k <= n; k++)
{
	for (c=1;c<=space;c++)
		printf(" ");
	 
	space--;
    
    for (c=1;c<=2*(k-1);c++)
		printf("*");

    printf("\n");
}
space =1;
for (k = 1; k <= n-1; ++k)
{
	for (c=1;c<=space;c++)
		printf(" ");
	 
	space++;
    
    for (c=1;c<=2*(n-k)-1;c++)
		printf("*");

    printf("\n");
}

}
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

	printf("esta es una prueba de libreria para elf ejecutables %d\n",argc);
	for (int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
	asus(20);
	asus2(20);
	syscall_pin_mode(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    while(1){
       syscall_pin_set(GPIOx(GPIO_C),13, 1);
       syscall_sleep(200);
       syscall_pin_set(GPIOx(GPIO_C),13, 0);
       syscall_sleep(100);
    }
    return 0;
}