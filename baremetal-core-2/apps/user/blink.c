
#include <stdio.h>
#include <syscall.h>
#include <gpio.h>

struct	cmdent	{			/* Entry in command table	*/
	char	cname[16];			/* Name of command		*/
	//bool8	cbuiltin;		/* Is this a builtin command?	*/
	int	(*cfunc)(int,char*[]);/* Function for command		*/
};


const	struct	cmdent	cmdtab[] = {
	{"elf",	NULL},
	{"bin",	NULL},
};
uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

char buff[100];
char *p;

int y=99;
int *u;
int main(int argc, char *argv[])
{

char *str="veronica garcia";
strcpy(buff,"daniel quintero");
p=&buff;
printf("%s %s %s\n",p,buff,str );
u=&y;
printf("%d %d\n",u,y );

 for (int i=0; i<ncmd; i++) {
    	printf("->%s\n", cmdtab[i].cname);
    }


    syscall_pin_mode(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    while(1){
       syscall_pin_set(GPIOx(GPIO_C),13, 1);
       syscall_sleep(200);
       syscall_pin_set(GPIOx(GPIO_C),13, 0);
       syscall_sleep(100);
    }
    return 0;
}