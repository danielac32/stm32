/* xsh_memdump.c - xsh_memdump */

#include <xinu.h>

 
/*------------------------------------------------------------------------
 * xsh_memdump - dump a region of memory by displaying values in hex
 *			 and ascii
 *------------------------------------------------------------------------
 */


 

shellcmd xsh_memdump(int nargs, char *args[])
{
	
long from = 0;
long to = 0x8FF;		/* AVR atmega328p top RAM address */

 if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s memdump [text,data,bss]\n\n", args[0]);
		printf("Description:\n");
		
		return OK;
	}
    printf("\n");
	if(!strcmp(args[1],"text")){
		printf("text\n");
        from = (uint32)&text;//atol(args[1]);
		to = (uint32)&etext;//atol(args[2]);
	}else if(!strcmp(args[1],"data")){
		printf("data\n");
        from = (uint32)&data;//atol(args[1]);
		to = (uint32)&edata;//atol(args[2]);
	}else if(!strcmp(args[1],"bss")){
		printf("bss\n");
        from = (uint32)&bss;//atol(args[1]);
		to = (uint32)&ebss;//atol(args[2]);
	}
    

    hexdump((void *)from,to-from,TRUE);


	return 0;
}
