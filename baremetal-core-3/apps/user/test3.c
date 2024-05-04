#include <string.h>
#include <stdio.h>
#include <syscall.h>
#include <gpio.h>


int main(int argc, char *argv[])
{

   printf("%s\n",argv[0] );
   while(1){
      	char *buff=syscall_malloc(1000);
   		memset(buff,0,1000);
   		syscall_free(buff);
   }
   return 0;
}