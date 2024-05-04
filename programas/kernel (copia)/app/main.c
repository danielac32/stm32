

#include <string.h>

char *str="daniel";
char *str2="quintero";
int y=7777;

char m[100];

int main(){
    char *p;

    strcpy(m,str2);
    
    p=m;

    *p++;

	asm ("mov r0, %0" : : "r" (y));  
	asm ("mov r1, %0" : : "r" ((unsigned int)str));   
	asm("svc 2");
	while(1){
		
		asm("nop");
	}
	return 0;
}