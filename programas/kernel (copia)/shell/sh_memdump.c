/* xsh_memdump.c - xsh_memdump */

#include <os.h>


extern unsigned int _text;
extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;
extern unsigned int _min_stack_sz;

/*------------------------------------------------------------------------
 * xsh_memdump - dump a region of memory by displaying values in hex
 *			 and ascii
 *------------------------------------------------------------------------
 */
shellcmd xsh_memdump(int nargs, char *args[])
{
	long from = 0;
	long to = 0x8FF;		/* AVR atmega328p top RAM address */

	/*if (nargs == 3) {
		from = atol(args[1]);
		to = atol(args[2]);
	}else{
		printf("default\n");
		from = (uint32)&_text;//atol(args[1]);
		to = (uint32)&_etext;//atol(args[2]);
	}
*/
    
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s memdump [text,data,bss]\n\n", args[0]);
		printf("Description:\n");
		
		return OK;
	}
    printf("\n");
	if(!strcmp(args[1],"text")){
		printf("text\n");
        from = (uint32)&_text;//atol(args[1]);
		to = (uint32)&_etext;//atol(args[2]);
	}else if(!strcmp(args[1],"data")){
		printf("data\n");
        from = (uint32)&_data;//atol(args[1]);
		to = (uint32)&_edata;//atol(args[2]);
	}else if(!strcmp(args[1],"bss")){
		printf("bss\n");
        from = (uint32)&_bss;//atol(args[1]);
		to = (uint32)&_ebss;//atol(args[2]);
	}
    

    hexdump((void *)from,to-from,true);
	/*long i;
	int j=0;
	char * c = 0;

	//put_dump((unsigned char *)from,0,to,DW_CHAR);
	for (i = from; i < to ; i++) {
		c = (char *)i;
		if (j==0) {
			printf("\n");
			printf ("0x%08x  ", c);
		}
		j++;
		if (j==16) j=0;
		if (*c < 33)
			printf("-");
		else
			printf("%c", *c);

	}
	printf("\n");
    printf("..............\n");*/

    //kill(getpid());
	return 0;
}
